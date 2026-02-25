// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CharacterSystem.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UCharacterSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// 子系统初始化
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 子系统销毁
	virtual void Deinitialize() override;
	

	//获取本地角色GUID
	FGuid GetLocalCharacterGUID();
	
	
};
