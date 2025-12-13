// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/SandBoxAnimNotify_BeHitStart.h"

void USandBoxAnimNotify_BeHitStart::SandBoxNotify(ASandBoxPeople* SandBoxPeople)
{
	Super::SandBoxNotify(SandBoxPeople);
	//应用已预存的伤害
	SandBoxPeople->ApplyDamage();
	//受击
	SandBoxPeople->OnBeHit();
	//应用切换到上一个状态
	SandBoxPeople->SwithToPreviousState();
}
