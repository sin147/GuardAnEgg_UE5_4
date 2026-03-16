// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkProxyActor/CharacterProxyActor.h"
#include "System/CharacterSubsystem.h"

// Sets default values
ACharacterProxyActor::ACharacterProxyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterProxyActor::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetCharacterProxy(this);
}

// Called every frame
void ACharacterProxyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

