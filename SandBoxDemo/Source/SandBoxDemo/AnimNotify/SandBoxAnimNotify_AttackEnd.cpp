// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/SandBoxAnimNotify_AttackEnd.h"

void USandBoxAnimNotify_AttackEnd::SandBoxNotify(ASandBoxPeople* SandBoxPeople)
{
	Super::SandBoxNotify(SandBoxPeople);
	//判断当前攻击人物是否还有效
	ASandBoxPeople* CurrentlyAttackingEnemy = SandBoxPeople->GetCurrentlyAttackingEnemy();
	if (!IsValidPeople(CurrentlyAttackingEnemy))
	{
		if (!SandBoxPeople->ManualDetectionAttackEnemy())
		{
			//若敌人无效停止攻击继续前进
			SandBoxPeople->StopAttack();
			SandBoxPeople->SwitchToMoveState();
		}
	}

}
