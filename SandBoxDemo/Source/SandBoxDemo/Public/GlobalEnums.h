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

//角色属性
UENUM(BlueprintType)
enum ECharacterAttribute : uint8
{
	//血量
	HP UMETA(DisplayName = "血量"),

	//移动速度
	MoveSpeed UMETA(DisplayName = "行走速度"),

	//转向速度
	RotatorSpeed UMETA(DisplayName = "转向速度")

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