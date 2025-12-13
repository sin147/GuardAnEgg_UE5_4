// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/SandBoxAnimNotify_DeathStart.h"

void USandBoxAnimNotify_DeathStart::SandBoxNotify(ASandBoxPeople* SandBoxPeople)
{
	Super::SandBoxNotify(SandBoxPeople);
	//禁用所有碰撞
	SandBoxPeople->SetActorEnableCollision(false);
	//关闭移动
	SandBoxPeople->StopMove();

}
