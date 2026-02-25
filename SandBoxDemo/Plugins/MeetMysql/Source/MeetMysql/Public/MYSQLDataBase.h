// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#define STATIC_CONCPP
#include "mysql_driver.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include <sstream>
#include "MYSQLDataBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MEETMYSQL_API UMYSQLDataBase : public UObject
{
	GENERATED_BODY()

private:
	sql::mysql::MySQL_Driver* driver;
	sql::Connection* con;
	sql::Statement* stamt;

	
public:
	
	/**
	 * 链接到指定的Mysql
	 * @param host 地址
	 * @param user 用户名
	 * @param pass 密码
	 * @param port 端口
	 * @return 是否成功链接
	 */
	UFUNCTION(BlueprintCallable,Category="MeetMysql")
	bool Connect(FString host,FString user,FString pass,int port);

	/**
	 * 执行无返回的sql语句
	 * @param sql sql语句
	 * @return 是否成功
	 */
	UFUNCTION(BlueprintCallable,Category="MeetMysql")
	bool Execute(FString sql);
	
	/**
	 * 执行查询语句
	 * @param sql sql语句
	 * @param resultStr 以字符串的形式返回查询结果，结构为  [列名1]:[列值1],[列名2]:[列值2]...
	 * @return 是否成功
	 */
	UFUNCTION(BlueprintCallable,Category="MeetMysql")
	bool ExecuteQuery(FString sql,TArray<FString>& resultStr);

	/**
	 * 执行增加、删除、修改语句
	 * @param sql sql语句
	 * @return 是否成功
	 */
	UFUNCTION(BlueprintCallable,Category="MeetMysql")
	bool ExecuteUpdate(FString sql);

	/**
	 * 选择数据库
	 * @param databaseName 数据库名
	 * @return 是否成功
	 */
	UFUNCTION(BlueprintCallable,Category="MeetMysql")
	bool SelectDataBase(FString databaseName);
	
};


