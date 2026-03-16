// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageProxyActor.generated.h"

UCLASS()
class SANDBOXDEMO_API ADamageProxyActor : public AActor
{
	GENERATED_BODY()
	friend class UDamageSubsytem;
public:	
	// Sets default values for this actor's properties
	ADamageProxyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
