// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_CharacterInfo.h"
#include "../SandBoxCharacter.h"
float UUI_CharacterInfo::UpdateCharacterHPBar()
{

	if (OwnerCharacter)
	{
		return (OwnerCharacter->GetCurrentlyHP()/OwnerCharacter->GetMaxHP());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UI的OwnerCharacter无效"));
		return 0;
	}

}

float UUI_CharacterInfo::UpdateKingHPBar()
{


	if (OwnerCharacter&&OwnerCharacter->GetKing())
	{
		
		return (OwnerCharacter->GetKing()->GetCurrentlyHP()/ OwnerCharacter->GetKing()->GetMaxHP());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UI的OwnerCharacter无效"));
		return 0;
	}
}

float UUI_CharacterInfo::UpdateSoldier1Bar()
{
	return GetSoldierSpawnProgress(0);
}

float UUI_CharacterInfo::UpdateSoldier2Bar()
{
	return GetSoldierSpawnProgress(1);
}

float UUI_CharacterInfo::UpdateSoldier3Bar()
{
	return GetSoldierSpawnProgress(2);
}

float UUI_CharacterInfo::UpdateSoldier4Bar()
{
	return GetSoldierSpawnProgress(3);
}

float UUI_CharacterInfo::GetSoldierSpawnProgress(int SoldierIndex)
{
	if (OwnerCharacter)
	{
		//获取king
		ASandBoxPeople* OwnerKing = OwnerCharacter->GetKing();
		FTimerHandle SpawnSoldier1;
		if (!OwnerKing)
		{
			return 0;
		}
		//获取士兵的生成的定时器
		SpawnSoldier1 = OwnerCharacter->GetKing()->GetSpawnSoldierTimersByIndex(SoldierIndex);
		//获取士兵生成时间
		float SpawnInterval = OwnerCharacter->GetKing()->GetSpawnSoldierIntervalByIndex(SoldierIndex);
		if (SpawnSoldier1.IsValid())
		{
			return 1-(GetWorld()->GetTimerManager().GetTimerRemaining(SpawnSoldier1) / SpawnInterval);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UI的OwnerCharacter无效"));
	
	}
	return 0;
}
