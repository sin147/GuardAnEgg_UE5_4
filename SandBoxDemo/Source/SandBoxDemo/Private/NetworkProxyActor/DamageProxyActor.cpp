// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkProxyActor/DamageProxyActor.h"
#include "System/DamageSubsytem.h"


// Sets default values
ADamageProxyActor::ADamageProxyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetDormancy = DORM_Never;
}

// Called when the game starts or when spawned
void ADamageProxyActor::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UDamageSubsytem>()->SetDamageProxy(this);
}

void ADamageProxyActor::Server_TakeDamage_Implementation(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	GetGameInstance()->GetSubsystem<UDamageSubsytem>()->Server_TakeDamage(DamagedActor, DamageAmount, DamageCauser);
}

void ADamageProxyActor::Multicast_TakeDamage_Implementation(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	GetGameInstance()->GetSubsystem<UDamageSubsytem>()->Multicast_TakeDamage(DamagedActor, DamageAmount, DamageCauser);
}

// Called every frame
void ADamageProxyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

