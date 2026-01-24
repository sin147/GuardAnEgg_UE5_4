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
	enum ECharacterState : uint8
	{
		Walk UMETA(DisplayName="行走"),
		Run UMETA(DisplayName="跑"),
		Idle UMETA(DisplayName="待机"),
		Interactive UMETA(DisplayName="交互"),
		Other UMETA(DisplayName="其他"),
	};
}

