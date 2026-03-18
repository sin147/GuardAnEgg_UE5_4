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
	TObjectPtr<ACharacterProxyActor> CharacterProxy;
public:
	//切换运动状态
	UFUNCTION(BlueprintCallable, Category = "Character")
	void ChangeMovementMode(AActor* InActor,EMovementMode MovementMode);
	//获取运动状态
	UFUNCTION(BlueprintCallable, Category = "Character")
	EMovementMode GetMovementMode(AActor* InActor);

protected:
	//设置角色代理
	void SetCharacterProxy(ACharacterProxyActor* InCharacterProxy);

	//服务器切换运动状态
	void Server_ChangeMovementMode(AActor* InActor, EMovementMode InMovementMode);

	//客户端切换运动状态
	void Multicast_ChangeMovementMode(AActor* InActor, EMovementMode InMovementMode);
};
