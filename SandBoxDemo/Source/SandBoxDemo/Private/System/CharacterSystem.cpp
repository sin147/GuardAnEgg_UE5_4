// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CharacterSystem.h"

void UCharacterSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance != nullptr)
	{
		//GameInstance
	}

}

void UCharacterSystem::Deinitialize()
{
}

FGuid UCharacterSystem::GetLocalCharacterGUID()
{
	//if(GetWorld()->GetFirstPlayerController()==nullptr&&GetWorld()->GetFirstPlayerController()->GetPawn()==nullptr)

	//	return GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorGuid();
	return FGuid();

}
