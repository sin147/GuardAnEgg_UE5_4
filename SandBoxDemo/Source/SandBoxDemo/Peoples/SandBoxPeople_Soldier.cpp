// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxPeople_Soldier.h"

void ASandBoxPeople_Soldier::OnAttackDetected(ASandBoxPeople* DetectedPeople)
{
	Super::OnAttackDetected(DetectedPeople);
		//更新当前攻击锁定的敌人
		CurrentlyAttackingEnemy = DetectedPeople;
		//判断攻击动画是否存在
		if (!AnimMontages.Contains(EPeopleState::Attack) || !AnimMontages[EPeopleState::Attack])
		{
			UE_LOG(LogTemp, Warning, TEXT(" %s,Attack animation not found!"),*GetName());
			return;
		}

		//切换到攻击状态
		SwitchToAttackState();
}

void ASandBoxPeople_Soldier::OnEnemyDetected(ASandBoxPeople* DetectedPeople)
{
	Super::OnEnemyDetected(DetectedPeople);
}

ASandBoxPeople_Soldier::ASandBoxPeople_Soldier()
{
}

void ASandBoxPeople_Soldier::InitSoldier(EPeopleCamp InCamp, TArray<EPeopleCamp> InEnemyTypes, TArray<EPeopleCamp> InAllyTypes, ASandBoxPeople* InEnemyKing)
{
	CurrentCamp = InCamp;
	EnemyTypes = InEnemyTypes;
	AllyTypes = InAllyTypes;
	EnemyKing = InEnemyKing;
}

