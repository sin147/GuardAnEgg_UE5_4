// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MYSQLDataBase.h"
#include "LogSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ULogSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	ULogSubsystem();
	//数据库
	TObjectPtr<UMYSQLDataBase> LogSql;
	
	//玩家登录
	UFUNCTION(BlueprintCallable, Category = "LogSubsystem")
	bool PlayerLogin(FString username, FString password);


};
