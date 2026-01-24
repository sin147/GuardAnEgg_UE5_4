// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "UI_CharacterInfo.generated.h"
class ASandBoxCharacter;
/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UUI_CharacterInfo : public UUserWidget
{
	GENERATED_BODY()
public:
	/*void Initialize()
	{
		OwnerCharacter = InOwnerCharacter;
	}*/

protected:
	//更新角色血条
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float UpdateCharacterHPBar();
	//更新王的血条
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float UpdateKingHPBar();
	//更新士兵进度条
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float UpdateSoldier1Bar();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float UpdateSoldier2Bar();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float UpdateSoldier3Bar();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float UpdateSoldier4Bar();
	//根据索引获取士兵生成进度条
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSoldierSpawnProgress(int SoldierIndex);

protected:
	//所属角色
	UPROPERTY(EditAnywhere)
	ASandBoxCharacter* OwnerCharacter;
	//玩家血量条
	UPROPERTY(meta = (BindWidget))
	UProgressBar* CharacterHPBar;
	//王的血量条
	UPROPERTY(meta = (BindWidget))
	UProgressBar* KingHPBar;
	//士兵1生成进度条
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Soldier1Bar;
	//士兵2生成进度条
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Soldier2Bar;
	//士兵3生成进度条
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Soldier3Bar;
	//士兵4生成进度条
	UPROPERTY(meta = (BindWidget))
	UProgressBar* Soldier4Bar;
};
