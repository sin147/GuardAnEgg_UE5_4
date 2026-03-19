// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMovementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SANDBOXDEMO_API IMovementInterface
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//设置运动Mode
	virtual void SetMoveMode(EMovementMode InNewMode)=0;

	//获取运动Mode
	virtual EMovementMode GetMoveMode()=0;

	//设置最大运动速度
	virtual void SetMoveMaxSpeed(float InSpeed,EMovementMode InMovementMode=EMovementMode::MOVE_None)=0;

	//获取最大运动速度
	virtual float GetMoveMaxSpeed(EMovementMode InMovementMode= EMovementMode::MOVE_None) = 0;
};
