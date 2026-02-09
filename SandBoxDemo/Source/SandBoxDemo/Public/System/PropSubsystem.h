// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Datas/PropDataAsset.h"
#include "Prop/PropBase.h"
#include "PropSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UPropSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPropSubsystem();

protected:
	//道具数据资产
	TObjectPtr<UPropDataAsset> PropDataAsset;

	//道具组
	TMap<FGuid, TObjectPtr<APropBase>> Props;
public:
	//掉落道具
	UFUNCTION(BlueprintCallable, Category = "Prop")
	bool DropProp(int PropID,FVector Location);
	//删除道具
	UFUNCTION(BlueprintCallable, Category = "Prop")
	void DeleteProp(FGuid PropGUID);
	//获取道具
	UFUNCTION(BlueprintCallable, Category = "Prop")
	APropBase* GetProp(FGuid PropGUID);
};
