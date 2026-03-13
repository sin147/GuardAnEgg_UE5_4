// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkProxyActor/InteractiveProxyActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/InteractiveSubsystem.h"

// Sets default values
AInteractiveProxyActor::AInteractiveProxyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetDormancy = DORM_Never;
}

// Called when the game starts or when spawned
void AInteractiveProxyActor::BeginPlay()
{
	Super::BeginPlay();
	if (Owner == GetWorld()->GetFirstPlayerController())
	{
		GetGameInstance()->GetSubsystem<UInteractiveSubsystem>()->SetInteractiveProxy(this);
	}

}

void AInteractiveProxyActor::Server_Interact_Implementation(ACharacter* InCharacter)
{
	 GetGameInstance()->GetSubsystem<UInteractiveSubsystem>()->Server_Interact(InCharacter);
}

void AInteractiveProxyActor::Muticast_Interact_Implementation(AActor* InActor, ACharacter* InCharacter)
{
	GetGameInstance()->GetSubsystem<UInteractiveSubsystem>()->Multicast_Interact(InActor,InCharacter);
}

void AInteractiveProxyActor::Muticast_OnSpawnInteractiveActor_Implementation(AActor* InActor)
{
	GetGameInstance()->GetSubsystem<UInteractiveSubsystem>()->Multicast_OnSpawnInteractiveActor(InActor);
}
void AInteractiveProxyActor::Server_SpawnInteractiveActor_Implementation(TSubclassOf<AActor> ActorClass, FVector InLocation, FRotator InRotation)
{
	GetGameInstance()->GetSubsystem<UInteractiveSubsystem>()->Server_SpawnInteractiveActor(ActorClass, InLocation, InRotation);
}
// Called every frame
void AInteractiveProxyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

