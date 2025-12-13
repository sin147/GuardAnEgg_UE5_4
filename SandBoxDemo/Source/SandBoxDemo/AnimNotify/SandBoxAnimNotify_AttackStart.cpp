// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/SandBoxAnimNotify_AttackStart.h"

void USandBoxAnimNotify_AttackStart::SandBoxNotify(ASandBoxPeople* SandBoxPeople)
{
	Super::SandBoxNotify(SandBoxPeople);
	//停止移动
	SandBoxPeople->StopMove();
	//转向到敌人
	SandBoxPeople->RotateToEnemy();
}
