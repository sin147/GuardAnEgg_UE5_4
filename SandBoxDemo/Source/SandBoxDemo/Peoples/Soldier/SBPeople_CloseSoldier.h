// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Peoples/SandBoxPeople_Soldier.h"
#include "SBPeople_CloseSoldier.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASBPeople_CloseSoldier : public ASandBoxPeople_Soldier
{
	GENERATED_BODY()
protected:
	virtual  void OnAllyDetected(ASandBoxPeople* DetectedPeople) override;
};
