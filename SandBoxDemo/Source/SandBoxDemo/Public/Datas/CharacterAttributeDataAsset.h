// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GlobalEnums.h"
#include "CharacterAttributeDataAsset.generated.h"

//校色属性结构体
USTRUCT(BlueprintType)
struct FCharacterAttribute
{

	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ECharacterAttribute> Attribute;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Default;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinValue;
};

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UCharacterAttributeDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCharacterAttribute> Attributes;
};
