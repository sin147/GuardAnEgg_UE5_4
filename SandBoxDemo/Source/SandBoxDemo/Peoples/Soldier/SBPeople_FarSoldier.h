// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Peoples/SandBoxPeople_Soldier.h"
#include "SBPeople_FarSoldier.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASBPeople_FarSoldier : public ASandBoxPeople_Soldier
{
	GENERATED_BODY()
protected:
	//远程攻击道具类型
	UPROPERTY(EditAnywhere, Category = "People|Attack|FarAttack")
	TSubclassOf<ASandBox_Prop> FarAttackPropClass;
	virtual void AttackEnemy() override;
};
