// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GlobalEnums.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterDataAsset.generated.h"

//角色属性结构体
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
class SANDBOXDEMO_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	//正常移动和快速移动比例  --Todo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1"))
	float QuickMoveSpeedRateByMaxMoveSpeed=0.25;
	//奔跑体力消耗
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float QuickMoveEnduranceConsumptionValue=10;
	//属性配置
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TEnumAsByte<ECharacterAttribute>,FCharacterAttribute>  Attributes;
	//技能配置
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag,TSubclassOf<UGameplayAbility>>  Abilities;
	
	FCharacterAttribute& GetAttributeByEnum(ECharacterAttribute CharacterAttribute);
	TSubclassOf<UGameplayAbility> GetAbility(FGameplayTag AbilityTag);

	bool IsVaildAttribute(ECharacterAttribute CharacterAttribute);
	bool IsVaildAbility(FGameplayTag CharacterAttribute);

	//深拷贝
	static UCharacterDataAsset* DeepCopy(UCharacterDataAsset* Source);

};
