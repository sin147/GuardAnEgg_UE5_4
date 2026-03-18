// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkProxyActor/CharacterProxyActor.h"

// Sets default values
ACharacterProxyActor::ACharacterProxyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetDormancy = DORM_Never;
}

// Called when the game starts or when spawned
void ACharacterProxyActor::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetCharacterProxy(this);
}

void ACharacterProxyActor::Server_ChangeMovementMode_Implementation(AActor* InActor, EMovementMode InMovementMode)
{
	GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->Server_ChangeMovementMode(InActor,InMovementMode);
}

void ACharacterProxyActor::Multicast_ChangeMovementMode_Implementation(AActor* InActor, EMovementMode InMovementMode)
{
	GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->Multicast_ChangeMovementMode(InActor, InMovementMode);
}

// Called every frame
void ACharacterProxyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


