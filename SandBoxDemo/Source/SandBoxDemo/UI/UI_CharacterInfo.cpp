// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_CharacterInfo.h"
#include "../SandBoxCharacter.h"

float UUI_CharacterInfo::UpdateCharacterHPBar()
{
	return 0.0f;
}

float UUI_CharacterInfo::UpdateKingHPBar()
{
	return 0.0f;
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
	return 0;
}
