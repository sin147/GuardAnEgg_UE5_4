// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LogSubsystem.h"

ULogSubsystem::ULogSubsystem()
{
	//初始化数据库连接
	LogSql = NewObject<UMYSQLDataBase>();
	LogSql->Connect(TEXT("43.139.151.189"), TEXT("JapeZhu"), TEXT("2002319Ab@"),3306);
	LogSql->SelectDataBase(TEXT("GuardAnEgg"));
}

bool ULogSubsystem::PlayerLogin(FString username, FString password)
{
	//TODO 连接数据库验证用户名和密码
	TArray<FString> result;
	if (IsValid(LogSql) && LogSql->ExecuteQuery(FString::Printf(TEXT("SELECT * FROM UserInfo WHERE UserId='%s' AND Password='%s'"), *username, *password), result))
	{
		if (result.IsEmpty())
		{
			// 登录失败，用户名或密码错误
			UE_LOG(LogTemp, Warning, TEXT("Login failed for user: %s"), *username);
			return false;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Login successful for user: %s"), *username);
			return true;
		}
	}

    return false;
}
