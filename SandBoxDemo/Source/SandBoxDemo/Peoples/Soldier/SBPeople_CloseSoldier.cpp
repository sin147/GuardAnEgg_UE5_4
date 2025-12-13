// Fill out your copyright notice in the Description page of Project Settings.


#include "Peoples/Soldier/SBPeople_CloseSoldier.h"

void ASBPeople_CloseSoldier::OnAllyDetected(ASandBoxPeople* DetectedPeople)
{
	Super::OnAllyDetected(DetectedPeople);
	CollisionDetectionComponent->MoveIgnoreActors.Add(DetectedPeople);
}
