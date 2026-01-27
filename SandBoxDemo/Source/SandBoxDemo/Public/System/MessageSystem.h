// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MessageSystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FNativeEventDelegate);

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UMessageSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	//todo
public:
	//注册本地事件
	static BindNativeEvent(int32 MsgId, UObject* InObject, FName Function)
	{
		if (NativeEventMap.Find(MsgId) != nullptr)
		{
			NativeEventMap.Add(MsgId, new FNativeEventDelegate());
		}
		NativeEventMap[MsgId].AddUFunction(InObject, Function);
	}
	static TMap<int32, FNativeEventDelegate> NativeEventMap;
	
};
