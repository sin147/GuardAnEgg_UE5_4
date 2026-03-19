// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkProxyActor/AttributeProxyActor.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AAttributeProxyActor::AAttributeProxyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
	NetDormancy = DORM_Never;
}

// Called when the game starts or when spawned
void AAttributeProxyActor::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->SetAttributeProxy(this);
}


// Called every frame
void AAttributeProxyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AAttributeProxyActor::Server_SetAttributeByEnum_Implementation(AActor* InActor, EAttribute InAttribute, float InValue, ECAVType InAttributeValueType)
{
	GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->Server_SetAttributeByEnum(InActor,InAttribute,InValue,InAttributeValueType);
	if (InAttribute == EAttribute::WalkSpeed && InAttributeValueType == ECAVType::CAVT_Max)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(InValue));
	}

}
void AAttributeProxyActor::Multicast_SetAttributeByEnum_Implementation(AActor* InActor, EAttribute InAttribute, float InValue, ECAVType InAttributeValueType)
{
	GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->Multicast_SetAttributeByEnum(InActor, InAttribute, InValue, InAttributeValueType);
	if (InAttribute == EAttribute::WalkSpeed && InAttributeValueType == ECAVType::CAVT_Max)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(InValue));
	}
}