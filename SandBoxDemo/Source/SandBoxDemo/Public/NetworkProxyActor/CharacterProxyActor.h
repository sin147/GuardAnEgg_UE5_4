// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/CharacterSubsystem.h"
#include "CharacterProxyActor.generated.h"

UCLASS()
class SANDBOXDEMO_API ACharacterProxyActor : public AActor
{
	GENERATED_BODY()
	friend class UCharacterSubsystem;
public:	
	// Sets default values for this actor's properties
	ACharacterProxyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//服务器设置角色运动Mode
	UFUNCTION(Server,Reliable)
	void Server_ChangeMovementMode(AActor*InCharacter,EMovementMode InMovementMode );

	//广播设置角色移动Mode
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_ChangeMovementMode(AActor* InCharacter, EMovementMode InMovementMode);

	//服务器设置角色运动速度
	UFUNCTION(Server, Reliable)
	void Server_SetMoveMaxSpeed(AActor* InCharacter, EMovementMode InMovementMode,float InSpeed);
	//广播设置角色移动速度
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetMoveMaxSpeed(AActor* InCharacter, EMovementMode InMovementMode, float InSpeed);
	//服务器设置角色奔跑
	UFUNCTION(Server, Reliable)
	void Server_SetCharacterRun(AActor* InCharacter, bool bRun);
	//广播设置角色奔跑
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCharacterRun(AActor* InCharacter, bool bRun);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
