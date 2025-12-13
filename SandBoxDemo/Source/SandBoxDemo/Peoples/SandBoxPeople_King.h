// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SandBoxPeople.h"
#include "SandBoxPeople_Soldier.h"
#include "SandBoxPeople_King.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASandBoxPeople_King : public ASandBoxPeople
{
	GENERATED_BODY()
protected:
	//可生成的士兵类型
	UPROPERTY(EditFixedSize, EditAnywhere, Category = "King")
	TArray<TSubclassOf<ASandBoxPeople_Soldier>> SoldierClasses;
	//生成地点
	UPROPERTY(EditFixedSize,meta = (MakeEditWidget),EditAnywhere, Category = "King")
	TArray<FVector> SpawnPositions;
	//士兵们
	TArray<TObjectPtr<ASandBoxPeople_Soldier>> Soldiers;
	//士兵生成定时器
	UPROPERTY(EditAnywhere, Category = "SolderSetting")
	TArray<FTimerHandle> SoldierSpawnTimers;
	//生成间隔
	UPROPERTY(EditAnywhere, Category = "SolderSetting")
	TArray<float> SpawnIntervals = { 15.0f,20.0f,40.0f,60.0f };
public:
	//根据索引获取士兵定时器
	FTimerHandle GetSpawnSoldierTimersByIndex(int SpawnSoldierIndex)
	{
		if (SoldierSpawnTimers.IsValidIndex(SpawnSoldierIndex))
		{
			return SoldierSpawnTimers[SpawnSoldierIndex];

		}
		return FTimerHandle();
	}
	//根据索引获取士兵的生成时间
	float GetSpawnSoldierIntervalByIndex(int SpawnSoldierIndex)
	{
		if (SpawnIntervals.IsValidIndex(SpawnSoldierIndex))
		{
			return SpawnIntervals[SpawnSoldierIndex];

		}
		return  0;
	}

	ASandBoxPeople_King();
	//单次生成士兵
	UFUNCTION(BlueprintCallable, Category = "King")
	void SpawnSoldierByIndex(int SoldierClassIndex);
	//按索引和间隔循环生成士兵
	UFUNCTION(BlueprintCallable, Category = "King")
	void ForloopSpawnSoldierByIndex(int SoldierClassIndex);
	//停止循环士兵循环生成
	UFUNCTION(BlueprintCallable, Category = "King")
	void StopForloopSpawnSoldierByIndex(int SoldierClassIndex);
};
