// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SandBoxGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASandBoxGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	ASandBoxGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
