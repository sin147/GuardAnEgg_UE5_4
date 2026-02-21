// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveSubsystem.h"
#include "InteractiveActor.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void UInteractiveSubsystem::RequestInteract(ACharacter* InCharacter)
{
	Server_Interact(InCharacter);
}

void UInteractiveSubsystem::PaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter, FGuid InInteractiveActorGUID)
{
	if (!InCharacter || !InInteractiveActorGUID.IsValid()) { return; }

	FCharacterInteractiveInfo*Info = CharacterInteractiveInfos.Find(InCharacter);
	if (Info==nullptr)
	{
		FCharacterInteractiveInfo NewInfo(InCharacter);
		CharacterInteractiveInfos.Add(InCharacter, NewInfo);
		Info = CharacterInteractiveInfos.Find(InCharacter);
	}
	Info->AddInteractiveActor(InInteractiveActorGUID);
}

void UInteractiveSubsystem::SpawnInteractiveActor(FVector InLocaltion,FRotator InRotation)
{

	AInteractiveActor*  NewInteractiveActor=GetWorld()->SpawnActor<AInteractiveActor>(InLocaltion,InRotation);
	InteractiveActors.Add(NewInteractiveActor);
	Multicast_OnSpawnInteractiveActor(NewInteractiveActor);
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

TArray<FGuid> UInteractiveSubsystem::FilterInteractiveActor(const TArray<FGuid>& InInteractiveActorGUID) const
{

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
