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
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
