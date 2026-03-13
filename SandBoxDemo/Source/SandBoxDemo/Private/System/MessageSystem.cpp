// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MessageSystem.h"
#include "Blueprint/BlueprintSupport.h"
#include "Engine/GameInstance.h"
#include "Engine/ServerStatReplicator.h"
#include "GameFramework/GameNetworkManager.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LevelScriptActor.h"
#include "Misc/CommandLine.h"
#include "Misc/PackageName.h"
#include "Net/OnlineEngineInterface.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameSession.h"
#include "Engine/NetConnection.h"
#include "Engine/ChildConnection.h"
#include "Engine/PlayerStartPIE.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
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

void UMessageSystem::SendToClient_Implementation(const FString& LocalPlayerId, ENetworkEventMessage InMessage)
{
	if (LocalPlayerId !=GetGameInstance()->GetFirstLocalPlayerController()->PlayerState->GetUniqueId().ToString())
	{
		return;
	}
	if (NetworkEventMap.Find(InMessage) == nullptr)
	{
		return;
	}
	NetworkEventMap[InMessage].Broadcast();
}
FString UMessageSystem::GetLocalPlayerId()
{
	return GetGameInstance()->GetFirstLocalPlayerController()->PlayerState->GetUniqueId().ToString();
}

void UMessageSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}
