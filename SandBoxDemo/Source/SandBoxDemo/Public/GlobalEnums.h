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
	//玩家运动状态
	S_Move UMETA(DisplayName = "移动"),
	S_QuickMove UMETA(DisplayName = "快速移动"),
	S_Idle UMETA(DisplayName = "待机"),
	S_Interactive UMETA(DisplayName = "交互"),

	//交互对象状态
	S_InteractiveActor_Pre UMETA(DisplayName = "交互对象准备交互"),
	S_InteractiveActor_Start UMETA(DisplayName = "交互对象开始交互"),
	S_InteractiveActor_Interacting UMETA(DisplayName = "交互对象交互中"),
	S_InteractiveActor_Break UMETA(DisplayName = "交互对象中断"),
	S_InteractiveActor_Finish UMETA(DisplayName = "交互对象完成"),
};

//本地事件枚举
UENUM(BlueprintType)
enum ENativeEventMessage :uint8
{
	//本地事件测
	Native_Test UMETA(DisplayName = "本地事件测试"),

};

//网络事件枚举
UENUM(BlueprintType)
enum ENetworkEventMessage:uint8
{
	//网络事件测试
	C2S_Test UMETA(DisplayName = "服务器到客户端测试"),
	S2C_Test UMETA(DisplayName = "客户端到服务器测试"),

};