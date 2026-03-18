// Fill out your copyright notice in the Description page of Project Settings.


#include "System/InteractiveSubsystem.h"
#include "Actor/InteractiveActor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void UInteractiveSubsystem::RequestInteract(ACharacter* InCharacter)
{
	ENetMode NetMode = GetWorld()->GetNetMode();
	if (NetMode == NM_ListenServer || NetMode == NM_DedicatedServer)
	{
		//TODO 代理到服务器
		InteractiveProxy->Muticast_Interact(InCharacter);
	}
	else
	{
		//TODO 代理到服务器
		InteractiveProxy->Server_Interact(InCharacter);
	}

}

bool UInteractiveSubsystem::PaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter,EInteractiveType InInteractiveType, FGuid InInteractiveActorGUID)
{
	if (!GetInteractiveActorByGUID(InInteractiveActorGUID)||!InCharacter || !InInteractiveActorGUID.IsValid()) { return false; }

	FCharacterInteractiveInfo*Info = CharacterInteractiveInfos.Find(InCharacter);
	if (Info==nullptr)
	{
		FCharacterInteractiveInfo NewInfo(InCharacter);
		CharacterInteractiveInfos.Add(InCharacter, NewInfo);
		Info = CharacterInteractiveInfos.Find(InCharacter);
	}
	return Info->AddInteractiveActor(InInteractiveType, InInteractiveActorGUID);
}

bool UInteractiveSubsystem::UnPaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter, FGuid InInteractiveActorGUID)
{
	if (!GetInteractiveActorByGUID(InInteractiveActorGUID)||!InCharacter || !InInteractiveActorGUID.IsValid()) { return false; }

	FCharacterInteractiveInfo* Info = CharacterInteractiveInfos.Find(InCharacter);
	if (Info == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::UnPaddingInteractiveActor CharacterGUID %s not found"), *InCharacter->GetActorNameOrLabel());
		return false ;
	}
	return Info->RemoveInteractiveActor(InInteractiveActorGUID);
}

void UInteractiveSubsystem::SpawnInteractiveActor(TSubclassOf<AActor> ActorClass,FVector InLocation,FRotator InRotation)
{
	if(IsValid(InteractiveProxy))
	{
		InteractiveProxy->Server_SpawnInteractiveActor(ActorClass, InLocation, InRotation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::SpawnInteractiveActor: InteractiveProxy is not valid! Cannot spawn interactive actor."));
	}
}

TArray<IInteract*> UInteractiveSubsystem::GetInteractiveActorsByGUIDs(TArray<FGuid> InGUIDs) const
{
	TArray<IInteract*> RetInteractiveActors;
	for (FGuid Guid : InGUIDs)
	{
		RetInteractiveActors.Add(GetInteractiveActorByGUID(Guid));
	}
	return RetInteractiveActors;
}

void UInteractiveSubsystem::DestoryInteractiveActorByGuid(FGuid Guid)
{
	IInteract* InteractiveActor = GetInteractiveActorByGUID(Guid);
	InteractiveActors.Remove(InteractiveActor);
}

void UInteractiveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UInteractiveSubsystem::SetInteractiveProxy(AInteractiveProxyActor* InProxy)
{
	if (InProxy)
	{
		InteractiveProxy = InProxy;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::SetInteractiveProxy: Invalid proxy actor!"));
	}
}

bool UInteractiveSubsystem::CanInteractFilter(ACharacter* InCharacter) const
{
	//是否存在可交互对象
	if (CharacterInteractiveInfos.Find(InCharacter) == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::RequestInteract CharacterGUID : %s not InteractiveActors"), *InCharacter->GetActorNameOrLabel());
		return false;
	}
	return true;
}

TArray<FGuid> UInteractiveSubsystem::FilterInteractiveActor(const TArray<FGuid> InInteractiveActorGUID) const
{
	TArray<FGuid> FilteredInteractiveActorGUIDs;
	return InInteractiveActorGUID;
}

void UInteractiveSubsystem::Server_Interact(ACharacter* InCharacter)
{
	FCharacterInteractiveInfo* Info = CharacterInteractiveInfos.Find(InCharacter);
	if (Info)
	{
		TArray<FGuid> ActiveInteractiveActorGuids = Info->GetInteractiveActorGUIDs(EInteractiveType::IT_Active);
		for (FGuid InteractiveActorGUID : ActiveInteractiveActorGuids)
		{
			IInteract* InteractiveActor = GetInteractiveActorByGUID(InteractiveActorGUID);
			if (InteractiveActor && InteractiveActor->CanInteract(InCharacter))
			{
				InteractiveActor->Interact(InCharacter);

			}
		}
		UE_LOG(LogTemp, Log, TEXT("UInteractiveSubsystem::RequestInteract CharacterGUID %s interact with %d actors"), *InCharacter->GetActorNameOrLabel(), ActiveInteractiveActorGuids.Num());
		InteractiveProxy->Muticast_Interact(InCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::RequestInteract CharacterGUID : %s not InteractiveActors"), *InCharacter->GetActorNameOrLabel());
	}
}

void UInteractiveSubsystem::Multicast_Interact(ACharacter* InCharacter)
{
	FCharacterInteractiveInfo* Info = CharacterInteractiveInfos.Find(InCharacter);
	if (Info)
	{
		TArray<FGuid> ActiveInteractiveActorGuids = Info->GetInteractiveActorGUIDs(EInteractiveType::IT_Active);
		for (FGuid InteractiveActorGUID : ActiveInteractiveActorGuids)
		{
			IInteract* InteractiveActor = GetInteractiveActorByGUID(InteractiveActorGUID);
			if (InteractiveActor && InteractiveActor->CanInteract(InCharacter))
			{
				InteractiveActor->Interact(InCharacter);
			}
		}
		UE_LOG(LogTemp, Log, TEXT("UInteractiveSubsystem::RequestInteract CharacterGUID %s interact with %d actors"), *InCharacter->GetActorNameOrLabel(), ActiveInteractiveActorGuids.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::RequestInteract CharacterGUID : %s not InteractiveActors"), *InCharacter->GetActorNameOrLabel());
	}
}

void UInteractiveSubsystem::Tick(float DeltaTime)
{
	////被动交互对象交互(服务器下执行)
	//if (GetWorld() ? (GetWorld()->GetNetMode() != NM_Client) : false)
	//{
	//	TArray<ACharacter*> CharacterArray;
	//	CharacterInteractiveInfos.GetKeys(CharacterArray);
	//	for (ACharacter* Character : CharacterArray)
	//	{
	//		TArray<FGuid>InteractiveActorGUIDs = CharacterInteractiveInfos[Character].GetInteractiveActorGUIDs(EInteractiveType::IT_Passive);
	//		TArray<IInteract*> PassiveInteractiveActors = GetInteractiveActorsByGUIDs(InteractiveActorGUIDs);
	//		for (IInteract* InteractiveActor : PassiveInteractiveActors)
	//		{
	//			InteractiveActor->Interact(Character);
	//		}

	//	}
	//}

	
}

ETickableTickType UInteractiveSubsystem::GetTickableTickType() const
{
	return ETickableTickType();
}

UWorld* UInteractiveSubsystem::GetTickableGameObjectWorld() const
{
	return nullptr;
}

TStatId UInteractiveSubsystem::GetStatId() const
{
	return TStatId();
}

void UInteractiveSubsystem::Multicast_OnSpawnInteractiveActor(AActor* NewInteractiveActor)
{
	IInteract* CastedInteractiveActor = Cast<IInteract>(NewInteractiveActor);
	if (CastedInteractiveActor!=nullptr&& InteractiveActors.Find(CastedInteractiveActor))
	{
		InteractiveActors.Add(CastedInteractiveActor);
	}
}

void UInteractiveSubsystem::Server_SpawnInteractiveActor(TSubclassOf<AActor> ActorClass, FVector InLocation, FRotator InRotation)
{
	// 1. 基础有效性检查：World/ActorClass 不能为空
	UWorld* World = GetWorld();
	if (!World || !IsValid(ActorClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::SpawnInteractiveActor: Invalid World or ActorClass!"));
		return;
	}

	// 2. 关键：检测 ActorClass 是否实现了 IInteract 接口（运行期反射检查）
	if (!ActorClass->ImplementsInterface(UInteract::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("UInteractiveSubsystem::SpawnInteractiveActor: ActorClass %s does NOT implement IInteract interface!"), *ActorClass->GetName());
		return;
	}

	// 3. 生成 Actor 对象
	AActor* NewActor = World->SpawnActor<AActor>(ActorClass, InLocation, InRotation);
	if (!NewActor)
	{
		UE_LOG(LogTemp, Error, TEXT("UInteractiveSubsystem::SpawnInteractiveActor: Failed to spawn Actor!"));
		return;
	}

	// 4. 二次验证：将 Actor 转换为 IInteract 接口指针（双重保险）
	IInteract* NewInteractiveActor = Cast<IInteract>(NewActor);
	if (NewInteractiveActor)
	{
		//InteractiveActors.Add(NewInteractiveActor); // 存入接口指针列表
		//InteractiveProxy->Muticast_OnSpawnInteractiveActor(NewActor); // 通知客户端更新交互对象列表
		UE_LOG(LogTemp, Log, TEXT("UInteractiveSubsystem::SpawnInteractiveActor: Spawned valid interactive actor %s"), *NewActor->GetName());
		InteractiveProxy->Muticast_OnSpawnInteractiveActor(NewActor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UInteractiveSubsystem::SpawnInteractiveActor: Actor %s implements IInteract but cast failed!"), *NewActor->GetName());
		NewActor->Destroy(); // 销毁无效 Actor
	}

}

void UInteractiveSubsystem::Multicast_SpawnInteractiveActor(AActor* AActor)
{
	IInteract* Interact = Cast<IInteract>(AActor);
	if (Interact)
	{
		if (!InteractiveActors.Contains(Interact))
		{
			InteractiveActors.Add(Interact);
		}
	}
}

FCharacterInteractiveInfo::FCharacterInteractiveInfo(TObjectPtr<ACharacter> InCharacter)
{
	Character = InCharacter;
	//TODO:先遍历一遍场景的InteractiveActor存储

}
