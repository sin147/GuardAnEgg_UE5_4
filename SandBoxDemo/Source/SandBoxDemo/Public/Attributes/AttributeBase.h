// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributeBase.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UAttributeBase : public UObject
{
	GENERATED_BODY()
protected:
	float MaxValue=100;
	float MinValue=-100;
	float CurrentlyValue;
public:

	//设置最大值
	void SetMaxValue(float NewValue)
	{
		MaxValue= NewValue;
	}
	//获取最大值
	float GetMaxValue()
	{
		return MaxValue;
	}

	//设置最小值
	void SetMinValue(float NewValue)
	{
		MinValue = NewValue;
	}
	//获取最小值
	float GetMinValue()
	{
		return MinValue;
	}

	//设置当前值
	void SetCurrentlyValue(float NewValue)
	{
		CurrentlyValue = FMath::Clamp(NewValue,MinValue,MaxValue);
	}
	//获取当前值
	float GetCurrentlyValue()
	{
		return CurrentlyValue;
	}
};
