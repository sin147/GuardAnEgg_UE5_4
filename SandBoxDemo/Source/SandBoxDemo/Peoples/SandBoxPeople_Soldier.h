// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SandBoxPeople.h"
#include "SandBoxPeople_Soldier.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASandBoxPeople_Soldier : public ASandBoxPeople
{
	GENERATED_BODY()
	
	
protected:
	//攻击逻辑
	virtual void OnAttackDetected(ASandBoxPeople* DetectedPeople) override;
	//索敌逻辑
	virtual void OnEnemyDetected(ASandBoxPeople* DetectedPeople) override;
public:
	ASandBoxPeople_Soldier();
	//初始化士兵
	void InitSoldier(EPeopleCamp InCamp,TArray<EPeopleCamp> InEnemyTypes,TArray<EPeopleCamp> InAllyTypes,ASandBoxPeople*InEnemyKing);
	
};
