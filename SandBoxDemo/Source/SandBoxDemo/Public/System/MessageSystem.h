// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlobalEnums.h"
#include "NetworkProxyActor/MessageProxyActor.h"
#include "MessageSystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FNativeEventDelegate);
DECLARE_MULTICAST_DELEGATE(FNetworkEventDelegate);

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UMessageSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	//todo
private:
	//代理Actor
	TObjectPtr<AMessageProxyActor> MessageProxyActor;

public:
/************************************本地事件************************************************/
	//本地事件映射
	 TMap<ENativeEventMessage, FNativeEventDelegate> NativeEventMap;
	//注册本地事件
	 FDelegateHandle BindNativeEvent(ENativeEventMessage InMessage, UObject* InObject, FName Function)
	{
		if (NativeEventMap.Find(InMessage) == nullptr)
		{
			NativeEventMap.Add(InMessage, FNativeEventDelegate());
		}
		return	NativeEventMap[InMessage].AddUFunction(InObject, Function);
	}
	//取消本地事件
	 bool UnBindNativeEventByHandle(ENativeEventMessage InMessage,FDelegateHandle InHandle)
	{
		if (NativeEventMap.Find(InMessage) != nullptr)
		{
			return NativeEventMap[InMessage].Remove(InHandle);
		}
		return false;
	}
	//激发本地事件
	 bool FireNativeEvent(ENativeEventMessage InMessage)
	{
		if (NativeEventMap.Find(InMessage) == nullptr)
		{
			return false;
		}
		NativeEventMap[InMessage].Broadcast();
		return true;
	}

/************************************网络事件************************************************/
	//网络事件注册
	 TMap<TEnumAsByte<ENetworkEventMessage>, FNetworkEventDelegate> NetworkEventMap;
	//注册网络事件
	 FDelegateHandle BindNetworkEvent(ENetworkEventMessage InMessage, UObject* InObject, FName Function)
	{
		if (NetworkEventMap.Find(InMessage) == nullptr)
		{
			NetworkEventMap.Add(InMessage, FNetworkEventDelegate());
		}
		return NetworkEventMap[InMessage].AddUFunction(InObject, Function);
	}
	//取消网络事件
	 bool UnBindNetworkEvent(TEnumAsByte<ENetworkEventMessage> InMessage, FDelegateHandle InHandle)
	{
		if (NetworkEventMap.Find(InMessage) != nullptr)
		{
			return	NetworkEventMap[InMessage].Remove(InHandle);
		}
		return false;
	}
	//发送到服务端
	 UFUNCTION(Server, Reliable,BlueprintCallable)
	 void SendToServer(ENetworkEventMessage InMessage);
	//发送到所有客户端
	 UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	 void SendToAllClients(ENetworkEventMessage InMessage);
	//发送到目标客户端
	 UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	 void SendToClient(const FString& LocalPlayerId,ENetworkEventMessage InMessage);
	 //打印当前玩家啊Id
	 UFUNCTION(BlueprintCallable)
	 FString GetLocalPlayerId();
	 virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
