// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LogSubsystem.h"

ULogSubsystem::ULogSubsystem()
{
	//初始化数据库连接
	LogSql = NewObject<UMYSQLDataBase>();
}

void ULogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LogSql->Connect(Host, User, Password, 3306);
	LogSql->SelectDataBase(DataBaseName);
}

bool ULogSubsystem::PlayerLogin(FString InUserID, FString InPassword)
{
	//TODO 连接数据库验证用户名和密码
	TArray<FString> result;
	if (IsValid(LogSql) && LogSql->ExecuteQuery(FString::Printf(TEXT("SELECT * FROM UserInfo WHERE UserId='%s' AND Password='%s'"), *InUserID, *InPassword), result))
	{
		if (result.IsEmpty())
		{
			// 登录失败，用户名或密码错误
			UE_LOG(LogTemp, Warning, TEXT("Login failed for user: %s"), *InUserID);
			return false;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Login successful for user: %s"), *InUserID);
			return true;
		}
	}
	else
	{
		//重新连接数据库
		LogSql->Connect(Host, User, Password, 3306);
		LogSql->SelectDataBase(DataBaseName);
		LogSql->ExecuteQuery(FString::Printf(TEXT("SELECT * FROM UserInfo WHERE UserId='%s' AND Password='%s'"), *InUserID, *InPassword), result);
			return true;
	}

    return false;
}

bool ULogSubsystem::PlayerRegister(FString InUserName, FString InUserID, FString InPassword)
{
	if (IsValid(LogSql) && LogSql->ExecuteUpdate(FString::Printf(TEXT("INSERT INTO UserInfo (UserId, Password, UserName) VALUES (%s,'%s','%s')"), *InUserID, *InPassword, *InUserName)))
	{
			UE_LOG(LogTemp, Log, TEXT("Register And Login successful for user: %s"), *InUserName);
			return true;
	}
	else
	{
		//重新连接数据库
		LogSql->Connect(Host, User, Password, 3306);
		LogSql->SelectDataBase(DataBaseName);
		LogSql->ExecuteUpdate(FString::Printf(TEXT("INSERT INTO UserInfo (UserId, Password, UserName) VALUES (%s,'%s','%s')"), *InUserID, *InPassword, *InUserName));
			return true;
	}
	UE_LOG(LogTemp, Log, TEXT("Register And Login failed for user: %s"), *InUserName);
	return false;
}
