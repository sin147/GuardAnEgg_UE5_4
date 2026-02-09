// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Prop/PropBase.h"
#include "PropDataAsset.generated.h"
USTRUCT(BlueprintType)
struct FPropConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APropBase> PropClass;
	//Value
	UPROPERTY(EditAnywhere)
	int Value;
};

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UPropDataAsset : public UDataAsset
{
	GENERATED_BODY()
	//道具Map
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FPropConfig> PropConfig;
	
};
