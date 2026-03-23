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
	//切换运动模式
	UFUNCTION(BlueprintCallable, Category = "Character")
	void ChangeMovementMode(AActor* InActor,EMovementMode MovementMode);
	//获取运动模式
	UFUNCTION(BlueprintCallable, Category = "Character")
	EMovementMode GetMovementMode(AActor* InActor);
	//设置角色最大运动速度
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetMoveMaxSpeed(AActor* InCharacter, EMovementMode InMovementMode, float InSpeed);
	//获取角色运动最大速度
	UFUNCTION(BlueprintCallable, Category = "Character")
	float GetMoveMaxSpeed(AActor* InCharacter, EMovementMode InMovementMode);
    //角色奔跑
    UFUNCTION(BlueprintCallable, Category = "Character")
    void SetCharacterRun(AActor* InCharacter, bool bRun);

protected:
	//设置角色代理
	void SetCharacterProxy(ACharacterProxyActor* InCharacterProxy);

	//服务器切换运动状态
	void Server_ChangeMovementModeImp(AActor* InActor, EMovementMode InMovementMode);

	//客户端切换运动状态
	void Multicast_ChangeMovementModeImp(AActor* InActor, EMovementMode InMovementMode);
	//服务器设置角色运动速度
	void Server_SetMoveMaxSpeedImp(AActor* InCharacter, EMovementMode InMovementMode, float InSpeed);

	//广播设置角色移动速度
	void Multicast_SetMoveMaxSpeedImp(AActor* InCharacter, EMovementMode InMovementMode, float InSpeed);
    //服务器设置角色奔跑
    void Server_SetCharacterRunImp(AActor* InCharacter, bool bRun);
	//广播设置角色奔跑
	void Multicast_SetCharacterRunImp(AActor* InCharacter, bool bRun);
};
