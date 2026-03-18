// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveProxyActor.generated.h"

UCLASS()
class SANDBOXDEMO_API AInteractiveProxyActor : public AActor
{
	GENERATED_BODY()

	friend class UInteractiveSubsystem;
public:	

	// Sets default values for this actor's properties
	AInteractiveProxyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//交互系统代理Actor
	UFUNCTION(Server, Reliable)
	void Server_Interact(ACharacter* InCharacter);
	//多播
	UFUNCTION(NetMulticast, Reliable)
	void Muticast_Interact(ACharacter* InCharacter);
	//多播生成代理交互actor
	UFUNCTION(NetMulticast, Reliable)
	void Muticast_OnSpawnInteractiveActor(AActor* InActor);
	//代理交互actor生成
	UFUNCTION(Server, Reliable)
	void Server_SpawnInteractiveActor(TSubclassOf<AActor> ActorClass, FVector InLocation, FRotator InRotation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
