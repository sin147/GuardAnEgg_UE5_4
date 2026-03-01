// Fill out your copyright notice in the Description page of Project Settings.


#include "System/InteractiveSubsystem.h"
#include "Actor/InteractiveActor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void UInteractiveSubsystem::RequestInteract(ACharacter* InCharacter)
{
	if (!CanInteractFilter(InCharacter)) { return; }
	Server_Interact(InCharacter);
}

bool UInteractiveSubsystem::PaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter, FGuid InInteractiveActorGUID)
{
	if (!InCharacter || !InInteractiveActorGUID.IsValid()) { return false; }

	FCharacterInteractiveInfo*Info = CharacterInteractiveInfos.Find(InCharacter);
	if (Info==nullptr)
	{
		FCharacterInteractiveInfo NewInfo(InCharacter);
		CharacterInteractiveInfos.Add(InCharacter, NewInfo);
		Info = CharacterInteractiveInfos.Find(InCharacter);
	}
	return Info->AddInteractiveActor(InInteractiveActorGUID);
}

bool UInteractiveSubsystem::UnPaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter, FGuid InInteractiveActorGUID)
{
	if (!InCharacter || !InInteractiveActorGUID.IsValid()) { return false; }

	FCharacterInteractiveInfo* Info = CharacterInteractiveInfos.Find(InCharacter);
	if (Info == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::UnPaddingInteractiveActor CharacterGUID %s not found"), *InCharacter->GetActorNameOrLabel());
		return false ;
	}
	return Info->RemoveInteractiveActor(InInteractiveActorGUID);
}

void UInteractiveSubsystem::SpawnInteractiveActor_Implementation(TSubclassOf<AInteractiveActor> ActorClass,FVector InLocaltion,FRotator InRotation)
{
	if (IsValid(ActorClass))
	{
		AInteractiveActor* NewInteractiveActor = GetWorld()->SpawnActor<AInteractiveActor>(ActorClass, InLocaltion, InRotation);
		InteractiveActors.Add(NewInteractiveActor);
		Multicast_OnSpawnInteractiveActor(NewInteractiveActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::SpawnInteractiveActor Invalid ActorClass"));
	}

}

AInteractiveActor* UInteractiveSubsystem::GetInteractiveActorByGUID(FGuid InGUID) const
{
	for (AInteractiveActor* InteractiveActor : InteractiveActors)
	{
		if (InteractiveActor && InteractiveActor->GetActorGuid() == InGUID)
		{
			return InteractiveActor;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::GetInteractiveActorByGUID InteractiveActor with GUID %s not found"), *InGUID.ToString());
	return nullptr;
}

bool UInteractiveSubsystem::CanInteractFilter(ACharacter* InCharacter) const
{
	//是否存在可交互对象
	if (CharacterInteractiveInfos.Find(InCharacter) == nullptr)
	{
		return false;
	}
	return true;
}

TArray<FGuid> UInteractiveSubsystem::FilterInteractiveActor(const TArray<FGuid> InInteractiveActorGUID) const
{
	TArray<FGuid> FilteredInteractiveActorGUIDs;
	//switch (EFilterType)
	//{
	//case FT_None:
	//	break;
	//	//角色交互只能与一个交互对象交互
	//case FT_Character:
	//	
	//	break;
	//	//设置交互可根据交互设置与多个交互对象交互
	//case FT_Setting:
	//	
	//	break;
	//default:
	//	break;
	//}

	return InInteractiveActorGUID;
}

void UInteractiveSubsystem::Server_Interact_Implementation(ACharacter* InCharacter)
{
	FCharacterInteractiveInfo* Info = CharacterInteractiveInfos.Find(InCharacter);
	if (Info)
	{
		TArray<FGuid> FilterInteractiveActorGUIDs = FilterInteractiveActor(Info->GetInteractiveActorGUIDs());
		for (FGuid InteractiveActorGUID : FilterInteractiveActorGUIDs)
		{
			if (GetInteractiveActorByGUID(InteractiveActorGUID))
			{
				GetInteractiveActorByGUID(InteractiveActorGUID)->Interact(InCharacter);
			}
		}
		UE_LOG(LogTemp, Log, TEXT("UInteractiveSubsystem::RequestInteract CharacterGUID %s interact with %d actors"), *InCharacter->GetActorNameOrLabel(), FilterInteractiveActorGUIDs.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractiveSubsystem::RequestInteract CharacterGUID %s not found"), *InCharacter->GetActorNameOrLabel());
	}
}

void UInteractiveSubsystem::Multicast_Interact_Implementation(ACharacter* InCharacter)
{
}

TArray<FGuid> UInteractiveSubsystem::CharacterFilterRule(TArray<FGuid> InInteractiveGUIDS)
{
	//获取相机的向前向量
	FVector CameraForwardVector = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector();
	TArray<FGuid> FilteredInteractiveGUIDs;
	TPair<FGuid, float> ClosestInteractive;
	for (FGuid InteractiveGUID:InInteractiveGUIDS )
	{
		//获取物品到相机的向量
		FVector ItemToCameraVector = GetInteractiveActorByGUID(InteractiveGUID)->GetActorLocation() - GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
		//计算物品与相机的夹角
		float dotProduct = FVector::DotProduct(CameraForwardVector.GetSafeNormal(), ItemToCameraVector.GetSafeNormal());
		//获取最小夹角的物品
		if (ClosestInteractive.Value< dotProduct)
		{
			ClosestInteractive.Key = InteractiveGUID;
			ClosestInteractive.Value = dotProduct;
			UE_LOG(LogTemp, Log, TEXT("UInteractiveSubsystem::CharacterFilterRule InteractiveGUID %s dotProduct %f"), *InteractiveGUID.ToString(), dotProduct);
		}

	}
	FilteredInteractiveGUIDs.Add(ClosestInteractive.Key);
	return FilteredInteractiveGUIDs;
}

void UInteractiveSubsystem::Tick(float DeltaTime)
{
	//if(CharacterInteractiveInfos[Getplayer])
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

void UInteractiveSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInteractiveSubsystem, InteractiveActors);
}

void UInteractiveSubsystem::Multicast_OnSpawnInteractiveActor_Implementation(AInteractiveActor* NewInteractiveActor)
{
	if (NewInteractiveActor)
	{
		InteractiveActors.Add(NewInteractiveActor);
	}
}

FCharacterInteractiveInfo::FCharacterInteractiveInfo(TObjectPtr<ACharacter> InCharacter)
{
	Character = InCharacter;
}
