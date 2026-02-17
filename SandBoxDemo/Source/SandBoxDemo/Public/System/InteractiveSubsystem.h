// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InteractiveSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UInteractiveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	//UPROPERTY(Replicated)
	//TMap<ACharacter*, TArray<int>> PreInteractiveActor;
public:
	//ÇëÇó½»»¥
	void RequestInteract(ACharacter* InCharacter);
	
	//½»»¥Âß¼­


	
	
};
