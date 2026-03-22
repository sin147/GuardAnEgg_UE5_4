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
		Server_ChangeMovementModeImp(InActor, InMovementMode);
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

void UCharacterSubsystem::SetMoveMaxSpeed(AActor* InActor, EMovementMode InMovementMode, float InSpeed)
{
	ENetMode NetMode = GetWorld()->GetNetMode();
	if (!IsValid(CharacterProxy)) { return; }
	if (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer)
	{
		Server_SetMoveMaxSpeedImp(InActor, InMovementMode, InSpeed);
	}
	else
	{
		CharacterProxy->Server_SetMoveMaxSpeed(InActor, InMovementMode, InSpeed);
	}
}

float UCharacterSubsystem::GetMoveMaxSpeed(AActor* InActor, EMovementMode InMovementMode)
{
	if (InActor != nullptr)
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InActor);
		if (MovementInterface != nullptr)
		{
			return MovementInterface->GetMoveMaxSpeed(InMovementMode);
		}
	}
	return 0;
}

void UCharacterSubsystem::SetCharacterProxy(ACharacterProxyActor* InCharacterProxy)
{
	if (GetWorld()->GetFirstPlayerController() == InCharacterProxy->Owner)
	{
		CharacterProxy = InCharacterProxy;
	}
}

void UCharacterSubsystem::Server_ChangeMovementModeImp(AActor* InActor, EMovementMode InMovementMode)
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

void UCharacterSubsystem::Multicast_ChangeMovementModeImp(AActor* InActor, EMovementMode InMovementMode)
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

void UCharacterSubsystem::Server_SetMoveMaxSpeedImp(AActor* InActor, EMovementMode InMovementMode, float InSpeed)
{
	if (InActor != nullptr)
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InActor);
		if (MovementInterface != nullptr)
		{
			MovementInterface->SetMoveMaxSpeed(InSpeed,InMovementMode);
			CharacterProxy->Multicast_SetMoveMaxSpeed(InActor,InMovementMode, InSpeed);
		}
	}
}

void UCharacterSubsystem::Multicast_SetMoveMaxSpeedImp(AActor* InActor, EMovementMode InMovementMode, float InSpeed)
{
	if (InActor != nullptr)
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InActor);
		if (MovementInterface != nullptr)
		{
			MovementInterface->SetMoveMaxSpeed(InSpeed,InMovementMode);
		}
	}
}
