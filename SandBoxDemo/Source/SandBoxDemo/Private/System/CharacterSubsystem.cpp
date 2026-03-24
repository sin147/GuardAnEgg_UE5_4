// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CharacterSubsystem.h"
#include "GameFramework/Character.h"
#include "Interface/MovementInterface.h"
#include "NetworkProxyActor/CharacterProxyActor.h"

void UCharacterSubsystem::ChangeMovementMode(AActor* InActor, EMovementMode InMovementMode)
{
	ENetMode NetMode = GetWorld()->GetNetMode();
	if (!IsValid(CharacterProxy)) { return; }
	switch (NetMode)
	{
	case NM_Standalone:
		Multicast_ChangeMovementModeImp(InActor, InMovementMode);
		break;
	case NM_DedicatedServer:
		Server_ChangeMovementModeImp(InActor, InMovementMode);
		break;
	case NM_ListenServer:
		CharacterProxy->Multicast_ChangeMovementMode(InActor, InMovementMode);
		break;
	case NM_Client:
		CharacterProxy->Server_ChangeMovementMode(InActor, InMovementMode);
		break;
	case NM_MAX:
		break;
	default:
		break;
	}
}

EMovementMode UCharacterSubsystem::GetMovementMode(AActor* InActor)
{
	if (IsValid(InActor))
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
	switch (NetMode)
	{
	case NM_Standalone:
		Multicast_SetMoveMaxSpeedImp(InActor, InMovementMode, InSpeed);
		break;
	case NM_DedicatedServer:
		Server_SetMoveMaxSpeedImp(InActor, InMovementMode, InSpeed);
		break;
	case NM_ListenServer:
		CharacterProxy->Multicast_SetMoveMaxSpeed(InActor, InMovementMode, InSpeed);
		break;
	case NM_Client:
		CharacterProxy->Server_SetMoveMaxSpeed(InActor, InMovementMode, InSpeed);
		break;
	case NM_MAX:
		break;
	default:
		break;
	}
}

float UCharacterSubsystem::GetMoveMaxSpeed(AActor* InActor, EMovementMode InMovementMode)
{
	if (IsValid(InActor))
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InActor);
		if (MovementInterface != nullptr)
		{
			return MovementInterface->GetMoveMaxSpeed(InMovementMode);
		}
	}
	return 0;
}

void UCharacterSubsystem::SetCharacterRun(AActor* InCharacter, bool bRun)
{
	ENetMode NetMode = GetWorld()->GetNetMode();
	if (!IsValid(CharacterProxy)) { return; }
	switch (NetMode)
	{
	case NM_Standalone:
		Multicast_SetCharacterRunImp(InCharacter, bRun);
		break;
	case NM_DedicatedServer:
		Server_SetCharacterRunImp(InCharacter, bRun);
		break;
	case NM_ListenServer:
		CharacterProxy->Multicast_SetCharacterRun(InCharacter, bRun);
		break;
	case NM_Client:
		CharacterProxy->Server_SetCharacterRun(InCharacter, bRun);

		break;
	case NM_MAX:
		break;
	default:
		break;
	}
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
	if (IsValid(InActor))
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
	if (IsValid(InActor))
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
	if (IsValid(InActor))
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
	if (IsValid(InActor))
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InActor);
		if (MovementInterface != nullptr)
		{
			MovementInterface->SetMoveMaxSpeed(InSpeed,InMovementMode);
		}
	}
}

void UCharacterSubsystem::Server_SetCharacterRunImp(AActor* InCharacter, bool bRun)
{
	if (IsValid(InCharacter))
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InCharacter);
		if (MovementInterface != nullptr)
		{
            MovementInterface->SetCharacterRun(bRun);
			CharacterProxy->Multicast_SetCharacterRun(InCharacter, bRun);
		}
	}
}

void UCharacterSubsystem::Multicast_SetCharacterRunImp(AActor* InCharacter, bool bRun)
{
	if (IsValid(InCharacter))
	{
		IMovementInterface* MovementInterface = Cast<IMovementInterface>(InCharacter);
		if (MovementInterface != nullptr)
		{
			MovementInterface->SetCharacterRun(bRun);
		}
	}
}
