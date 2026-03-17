// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "System/AttributeSubsystem.h"
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
	void SetValueByValueType(float InValue,ECAVType InValueType)
	{
		switch (InValueType)
		{
		case CAVT_Max:
			MaxValue = fmaxf(InValue, MinValue);
			break;
		case CAVT_Currently:
			Default = InValue > MaxValue ? MaxValue : InValue < MinValue ? MinValue : InValue;
			break;
		case CAVT_Min:
			MinValue = fminf(InValue, MaxValue);
			break;
		default:
			break;
		}

		
	}
	float GetValueByValueType(ECAVType InValueType)
	{
		switch (InValueType)
		{
		case CAVT_Max:
			return MaxValue;
		case CAVT_Currently:
			return Default;
		case CAVT_Min:
			return MinValue;
		default:
			break;
		}
		return 0;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1"), DisplayName = "快速移动速度比例")
	float QuickMoveSpeedRateByMaxMoveSpeed=0.25;
	//奔跑体力消耗
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1"), DisplayName = "快速移动体力消耗比例（每秒）")
	float QuickMoveEnduranceConsumptionValue=10;
	//属性配置
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TEnumAsByte<EAttribute>,FCharacterAttribute>  Attributes;
	//技能配置
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag,TSubclassOf<UGameplayAbility>>  Abilities;
	
	FCharacterAttribute& GetAttributeByEnum(EAttribute CharacterAttribute);
	TSubclassOf<UGameplayAbility> GetAbility(FGameplayTag AbilityTag);

	bool IsVaildAttribute(EAttribute CharacterAttribute);
	bool IsVaildAbility(FGameplayTag CharacterAttribute);

	//深拷贝
	static UCharacterDataAsset* DeepCopy(UCharacterDataAsset* Source);

};
