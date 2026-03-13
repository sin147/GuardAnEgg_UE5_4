// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkProxyActor/MessageProxyActor.h"
#include "System/MessageSystem.h"

// Sets default values
AMessageProxyActor::AMessageProxyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMessageProxyActor::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UMessageSystem>()->SetMessageProxyActor(this);
}

// Called every frame
void AMessageProxyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMessageProxyActor::PostNetInit()
{
}

