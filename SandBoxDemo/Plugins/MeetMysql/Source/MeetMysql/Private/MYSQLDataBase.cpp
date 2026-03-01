// Fill out your copyright notice in the Description page of Project Settings.


#include "MYSQLDataBase.h"
#include "Kismet/KismetSystemLibrary.h"



bool UMYSQLDataBase::Connect(FString host, FString user, FString pass, int port)
{
	// 先释放可能存在的旧连接
	if (stamt)
	{
		delete stamt;
		stamt = nullptr;
	}
	if (con)
	{
		delete con;
		con = nullptr;
	}
	driver = nullptr;

	
	try
	{
		// 验证输入参数有效性
		if (host.IsEmpty() || user.IsEmpty() || port <= 0 || port > 65535)
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid connection parameters: Host=%s, Port=%d, User=%s"), 
				  *host, port, *user);
			return false;
		}


		
		driver = sql::mysql::get_mysql_driver_instance();
		if (!driver)
		{
			return false;
		}

		
		std::string hostName(TCHAR_TO_UTF8(*host));
		std::string userName(TCHAR_TO_UTF8(*user));
		std::string Pass(TCHAR_TO_UTF8(*pass));
		
		// 构建带SSL模式的连接字符串
		std::string connectionString = "tcp://" + hostName + ":" + std::to_string(port) + "?ssl-mode=disable";

		// 建立连接
		con = driver->connect(connectionString, userName, Pass);

		if (!con)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create connection object"));
			return false;
		}

		// 创建语句对象
		stamt = con->createStatement();
		if (!stamt)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create statement object"));
			delete con;  // 释放已创建的连接
			con = nullptr;
			return false;
		}

		UE_LOG(LogTemp, Log, TEXT("Successfully connected to database"));
		return true;
	}
	catch (const sql::SQLException& sQLException)
	{
		UE_LOG(LogTemp, Error, TEXT("SQL Exception: %hs (Error Code: %d, SQL State: %hs)"),
			  sQLException.what(), sQLException.getErrorCode(), sQLException.getSQLStateCStr());
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("SQL Error: %hs"), sQLException.what()), 
										 true, true, FColor::Red);
	}
	catch (const std::exception& err)
	{
		UE_LOG(LogTemp, Error, TEXT("Standard Exception: %hs"), err.what());
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Error: %hs"), err.what()), 
										 true, true, FColor::Red);
	}
	catch (...)
	{
		UE_LOG(LogTemp, Error, TEXT("Unknown exception occurred during database connection"));
		UKismetSystemLibrary::PrintString(this, TEXT("Unknown error occurred"), true, true, FColor::Red);
	}

	// 异常发生后确保资源被释放
	if (stamt)
	{
		delete stamt;
		stamt = nullptr;
	}
	if (con)
	{
		delete con;
		con = nullptr;
	}
	driver = nullptr;

	return false;
}

bool UMYSQLDataBase::Execute(FString sql)
{
	try
	{
		std::string sqlString(TCHAR_TO_UTF8(*sql));
		if (!stamt)
		{
				UE_LOG(LogTemp, Error, TEXT("Failed to create statement object after reconnecting"));
				return false;
		}
		stamt->execute(sqlString);
		return true;
	}
	catch (const std::runtime_error err)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), err.what());
		UKismetSystemLibrary::PrintString(this, FString(err.what()),true,true,FColor::Red);
		return false;
	}
	catch (const sql::SQLException& sQLException)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), sQLException.what());
		UKismetSystemLibrary::PrintString(this, FString(sQLException.what()),true,true,FColor::Red);
		return false;
	}
	catch (const std::bad_alloc& bad_allocException)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), bad_allocException.what());
		UKismetSystemLibrary::PrintString(this, FString(bad_allocException.what()),true,true,FColor::Red);
		return false;
	}
}

bool UMYSQLDataBase::ExecuteQuery(FString sql,TArray<FString>& resultStr)
{
	try
	{
		std::string sqlString(TCHAR_TO_UTF8(*sql));
		
		sql::ResultSet *res;

		if (!stamt)
		{
				 UE_LOG(LogTemp, Error, TEXT("Failed to create statement object after reconnecting"));
				 return false;
		}
		res = stamt->executeQuery(sqlString);
		//TODO 返回查询结果
		// 获取结果集的元数据
		sql::ResultSetMetaData* res_meta = res->getMetaData();
		int num_columns = res_meta->getColumnCount();

		// 用于存储结果的字符串


		// 遍历结果集
		while (res->next()) {

			std::stringstream ss;
			// 遍历每一列
			for (int i = 1; i <= num_columns; ++i) {
				// 获取列名
				std::string column_name = res_meta->getColumnName(i);
				// 获取列值
				std::string column_value = res->getString(i);

				// 将列名和列值拼接到字符串流中
				ss << column_name << ":" << column_value;

				// 如果不是最后一列，添加分隔符
				if (i < num_columns) {
					ss << "|";
				}
			}

			// 每一行结束后换行
			//ss << "|";
			resultStr.Add(UTF8_TO_TCHAR(ss.str().c_str()));
		}
		// 将字符串流的内容转换为字符串
		delete res;
		return true;
	}
	catch (const std::runtime_error err)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), err.what());
		UKismetSystemLibrary::PrintString(this, FString(err.what()),true,true,FColor::Red);
		std::cout << err.what() << std::endl;
		return false;
	}
	catch (const sql::SQLException& sQLException)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"),  sQLException.what());
		UKismetSystemLibrary::PrintString(this, FString(sQLException.what()),true,true,FColor::Red);
		std::cout << sQLException.what() << std::endl;
		return false;
	}
	catch (const std::bad_alloc& bad_allocException)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), bad_allocException.what());
		UKismetSystemLibrary::PrintString(this, FString(bad_allocException.what()),true,true,FColor::Red);
		std::cout << bad_allocException.what() << std::endl;
		return false;
	}
}

bool UMYSQLDataBase::ExecuteUpdate(FString sql)
{
	try
	{
		std::string sqlString(TCHAR_TO_UTF8(*sql));
		if (!stamt)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create statement object after reconnecting"));
			return false;
		}
		stamt->executeUpdate(sqlString);
		return true;
	}
	catch (const std::runtime_error err)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), err.what());
		UKismetSystemLibrary::PrintString(this, FString(err.what()),true,true,FColor::Red);
		return false;
	}
	catch (const sql::SQLException& sQLException)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), sQLException.what());
		UKismetSystemLibrary::PrintString(this, FString(sQLException.what()),true,true,FColor::Red);
		return false;
	}
	catch (const std::bad_alloc& bad_allocException)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), bad_allocException.what());
		UKismetSystemLibrary::PrintString(this, FString(bad_allocException.what()),true,true,FColor::Red);
		return false;
	}
}

bool UMYSQLDataBase::SelectDataBase(FString databaseName)
{
	try
	{
		std::string sqlString(TCHAR_TO_UTF8(*databaseName));
		stamt->execute("USE "+sqlString);
		stamt->execute("SET NAMES utf8mb4;");
		return true;
	}
	catch (const std::runtime_error err)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), err.what());
		UKismetSystemLibrary::PrintString(this, FString(err.what()),true,true,FColor::Red);
		return false;
	}
	catch (const sql::SQLException& sQLException)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), sQLException.what());
		UKismetSystemLibrary::PrintString(this, FString(sQLException.what()),true,true,FColor::Red);
		return false;
	}
	catch (const std::bad_alloc& bad_allocException)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), bad_allocException.what());
		UKismetSystemLibrary::PrintString(this, FString(bad_allocException.what()),true,true,FColor::Red);
		return false;
	}
}


