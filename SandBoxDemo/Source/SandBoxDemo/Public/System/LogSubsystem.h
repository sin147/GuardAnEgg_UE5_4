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
private:
	FString Host= "43.139.151.189";
	FString User= "JapeZhu";
	FString Password= "2002319Ab@";
	int Port= 3306;
	FString DataBaseName = "GuardAnEgg";
public:
	ULogSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//数据库
	TObjectPtr<UMYSQLDataBase> LogSql;
	
	//玩家登录
	UFUNCTION(BlueprintCallable, Category = "LogSubsystem")
	bool PlayerLogin(FString InUserId, FString InPassword);

	//玩家登录
	UFUNCTION(BlueprintCallable, Category = "LogSubsystem")
	bool PlayerRegister(FString InUserName,FString InUserID, FString InPassword);


};
