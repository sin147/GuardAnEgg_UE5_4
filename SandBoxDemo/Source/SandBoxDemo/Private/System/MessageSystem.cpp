// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MessageSystem.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
//todo
void UMessageSystem::SendToServer_Implementation(ENetworkEventMessage InMessage)
{

	if (NetworkEventMap.Find(InMessage) == nullptr)
	{
		//GetGameInstance()->id
		return;
	}
	NetworkEventMap[InMessage].Broadcast();
}

void UMessageSystem::SendToAllClients_Implementation(ENetworkEventMessage InMessage)
{
	if (NetworkEventMap.Find(InMessage) == nullptr)
	{
		return;
	}
	NetworkEventMap[InMessage].Broadcast();
}

void UMessageSystem::SendToClient_Implementation(APlayerController*PlayerController, ENetworkEventMessage InMessage)
{
	if (PlayerController->PlayerState->GetUniqueId().ToString() != GetGameInstance()->GetFirstLocalPlayerController()->PlayerState->GetUniqueId())
	{
		return;
	}
	if (NetworkEventMap.Find(InMessage) == nullptr)
	{
		return;
	}
	NetworkEventMap[InMessage].Broadcast();
}
