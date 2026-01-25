// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SANDBOXDEMO_API GlobalEnums
{
public:
	GlobalEnums();
	~GlobalEnums();
};


namespace CharacterState
{
	UENUM(BlueprintType)
	enum ELayer : uint8
	{
		Movement UMETA(DisplayName = "移动"),
		Interactive UMETA(DisplayName = "交互"),
	};
	namespace Movement
	{
		UENUM(BlueprintType)
		enum EMovement : uint8
		{
			Move UMETA(DisplayName = "移动"),
			QuickMove UMETA(DisplayName = "快速移动"),
			Idle UMETA(DisplayName = "待机"),
		};
	}
	namespace Interactive
	{
		UENUM(BlueprintType)
		enum EInteractive : uint8
		{
			Collect UMETA(DisplayName = "采集");
		};
	}
}

