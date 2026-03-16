// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlobalEnums.h"
#include "CharacterSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UCharacterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	friend class ACharacterProxyActor;
private:
	//角色代理
	ACharacterProxyActor* CharacterProxy;
protected:
	//设置角色代理
	void SetCharacterProxy(ACharacterProxyActor* Proxy) { CharacterProxy = Proxy; }
	//服务器处理设置角色状态
	void Server_SetCharacterStateByEnum(ECharacterAttribute InAttribute, float InNewValue, ECAVType InValueType);
	//多播处理设置角色状态
	void Multicast_SetCharacterStateByEnum(ECharacterAttribute InAttribute, float InNewValue, ECAVType InValueType);

public:
	//设置玩家状态
	void SetCharacterStateByEnum(ECharacterAttribute InAttribute, float InNewValue, ECAVType InValueType);
	
};
