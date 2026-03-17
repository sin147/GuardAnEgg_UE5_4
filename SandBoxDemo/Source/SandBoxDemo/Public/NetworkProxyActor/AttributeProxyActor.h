// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/AttributeSubsystem.h"
#include "AttributeProxyActor.generated.h"

UCLASS()
class SANDBOXDEMO_API AAttributeProxyActor : public AActor
{
	GENERATED_BODY()
	friend class UAttributeSubsystem;
public:	
	// Sets default values for this actor's properties
	AAttributeProxyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//服务器设置属性
	UFUNCTION(Server,Reliable)
	void Server_SetAttributeByEnum(AActor* InActor, EAttribute InAttribute, float InValue, ECAVType InAttributeValueType);
	//多播设置值
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetAttributeByEnum(AActor* InActor, EAttribute InAttribute, float InValue, ECAVType InAttributeValueType);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
