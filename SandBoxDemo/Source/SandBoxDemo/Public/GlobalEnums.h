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



UENUM(BlueprintType)
enum EState : uint8
{
	S_None UMETA(DisplayName = "无"),
	S_Move UMETA(DisplayName = "移动"),
	S_QuickMove UMETA(DisplayName = "快速移动"),
	S_Idle UMETA(DisplayName = "待机"),
	S_Interactive UMETA(DisplayName = "交互"),
};

