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
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float MaxValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Default;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float MinValue;
public:
	void SetMaxValue(float InValue)
	{
		MaxValue = fmaxf(InValue,MinValue);
	}

	void SetMinValue(float InValue)
	{
		MinValue = fminf(InValue, MaxValue);
	}

	void SetCurrentlyValue(float InValue)
	{
		Default = InValue > MaxValue ? MaxValue : InValue < MinValue ? MinValue : InValue;
		//UE_LOG(LogTemp, Log, TEXT("SetYawRotatorSpeed %lf "), Default);

	}
	float GetMaxValue()
	{
		return MaxValue;
	}
	float GetMinValue()
	{
		return MinValue;
	}
	float GetCurrentlyValue()
	{
		//UE_LOG(LogTemp, Log, TEXT("GetYawRotatorSpeed %lf "), Default);
		return Default;
	}

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
	TMap<TEnumAsByte<ECharacterAttribute>,FCharacterAttribute>  Attributes;
	
	FCharacterAttribute& operator[](ECharacterAttribute CharacterAttribute);

	bool IsVaildKey(ECharacterAttribute CharacterAttribute);

};
