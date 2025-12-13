// Fill out your copyright notice in the Description page of Project Settings.


#include "Peoples/SandBoxPeople_King.h"
#include "Kismet/KismetMathLibrary.h"
ASandBoxPeople_King::ASandBoxPeople_King()
{
	for (int i = 0; i < 4; i++)
	{
		SpawnPositions.Add(FVector(0,0,0));
	}
	//预设4种士兵类型
	SoldierClasses.SetNum(4);
	//预设4个生成定时器
	SoldierSpawnTimers.Add(FTimerHandle());
	SoldierSpawnTimers.Add(FTimerHandle());
	SoldierSpawnTimers.Add(FTimerHandle());
	SoldierSpawnTimers.Add(FTimerHandle());
}

void ASandBoxPeople_King::SpawnSoldierByIndex(int SoldierClassIndex)
{	
	//随机生成位置
	int SpawnPositionIndex=FMath::RandRange(0, 3);
	//生成属性
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner=this;
	//生成新的士兵
	if (EnemyKing)
	{
		ASandBoxPeople_Soldier* NewSoldier = GetWorld()->SpawnActor <ASandBoxPeople_Soldier>(SoldierClasses[SoldierClassIndex], FTransform(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyKing->GetActorLocation()), GetActorLocation() + SpawnPositions[SpawnPositionIndex], FVector(1, 1, 1)), SpawnParameters);
		if (NewSoldier)
		{
			//初始化敌方King
			NewSoldier->InitSoldier(GetCurrentCamp(), EnemyTypes, AllyTypes, EnemyKing);
			NewSoldier->SwitchToMoveState();
			Soldiers.Add(NewSoldier);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Spawn Soldier Failed"));
		}
	}
	else
	{
		ASandBoxPeople_Soldier* NewSoldier = GetWorld()->SpawnActor <ASandBoxPeople_Soldier>(SoldierClasses[SoldierClassIndex], FTransform(FRotator(0,0,0), GetActorLocation() + SpawnPositions[SpawnPositionIndex], FVector(1, 1, 1)), SpawnParameters);
		if (NewSoldier)
		{
			//初始化敌方King
			Soldiers.Add(NewSoldier);
		}
		UE_LOG(LogTemp, Error, TEXT("未设置EnemyKing"));
	}
	
}

void ASandBoxPeople_King::ForloopSpawnSoldierByIndex(int SoldierClassIndex)
{
	//获取软引用
	TWeakObjectPtr<ASandBoxPeople_King> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(SoldierSpawnTimers[SoldierClassIndex], [WeakThis,SoldierClassIndex]() {
		ASandBoxPeople_King* This = WeakThis.Get();
		if (This)
		{
			This->SpawnSoldierByIndex(SoldierClassIndex);
		}
		}, SpawnIntervals[SoldierClassIndex], true);
}

void ASandBoxPeople_King::StopForloopSpawnSoldierByIndex(int SoldierClassIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(SoldierSpawnTimers[SoldierClassIndex]);
}

