// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/SandBoxAnimNotify_Attacking.h"

void USandBoxAnimNotify_Attacking::SandBoxNotify(ASandBoxPeople* SandBoxPeople)
{
	Super::SandBoxNotify(SandBoxPeople);
	//把攻击对像设置为受击
		SandBoxPeople->AttackEnemy();
}
