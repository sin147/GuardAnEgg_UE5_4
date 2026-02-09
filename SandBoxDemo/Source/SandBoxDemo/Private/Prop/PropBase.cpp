// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PropBase.h"

// Sets default values
APropBase::APropBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropMesh"));
	PropMesh->SetupAttachment(RootComponent);
	if (!PropInfo.GUID.IsValid())
	{
		PropInfo.GUID = FGuid::NewGuid();
	}
}

void APropBase::InitInfo(int InPropID)
{
	PropInfo.PropID = InPropID;
}

FGuid APropBase::GetGUID()
{
	return PropInfo.GUID;
}

int APropBase::GetPropID()
{
	return PropInfo.PropID;
}

// Called when the game starts or when spawned
void APropBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APropBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

