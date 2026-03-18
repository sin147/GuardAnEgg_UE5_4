// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CharacterSubsystem.h"
#include "GameFramework/Character.h"
#include "Interface/MovementInterface.h"
#include "NetworkProxyActor/CharacterProxyActor.h"

void UCharacterSubsystem::ChangeMovementMode(AActor* InActor, EMovementMode InMovementMode)
{
	ENetMode NetMode = GetWorld()->GetNetMode();
	if (!IsValid(CharacterProxy)) { return; }
	if (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer)
	{
		Server_ChangeMovementMode(InActor, InMovementMode);
	}
	else
	{
		CharacterProxy->Server_ChangeMovementMode(InActor, InMovementMode);
	}

}

EMovementMode UCharacterSubsystem::GetMovementMode(AActor* InActor)
{
	if (InActor != nullptr)
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InActor);
		if (MovementInterface != nullptr)
		{
			return MovementInterface->GetMoveMode();
		}
	}
	return EMovementMode::MOVE_None;
}

void UCharacterSubsystem::SetCharacterProxy(ACharacterProxyActor* InCharacterProxy)
{
	if (InCharacterProxy != nullptr)
	{
		CharacterProxy = InCharacterProxy;
	}
}

void UCharacterSubsystem::Server_ChangeMovementMode(AActor* InActor, EMovementMode InMovementMode)
{
	if (!IsValid(CharacterProxy)) { return; }
	if (InActor != nullptr)
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InActor);
		if (MovementInterface !=nullptr)
		{
			MovementInterface->SetMoveMode(InMovementMode);
			CharacterProxy->Multicast_ChangeMovementMode(InActor, InMovementMode);
		}

	}
}

void UCharacterSubsystem::Multicast_ChangeMovementMode(AActor* InActor, EMovementMode InMovementMode)
{
	if (InActor != nullptr)
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InActor);
		if (MovementInterface != nullptr)
		{
			MovementInterface->SetMoveMode(InMovementMode);
		}
	}
}
