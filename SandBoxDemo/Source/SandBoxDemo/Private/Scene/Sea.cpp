// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/Sea.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/BrushComponent.h"
void ASea::BeginPlay()
{
	Super::BeginPlay();
	InitalPhysicsVolume();
}

void ASea::InitalPhysicsVolume()
{
	UE_LOG(LogTemp, Log, TEXT("生成海洋PhysicsVolume"));
	WaterVolume = GetWorld()->SpawnActor<APhysicsVolume>(GetActorLocation(), GetActorRotation());
	WaterVolume->SetActorScale3D(GetBoxExtent()/100);
	WaterVolume->bWaterVolume = true;
}
