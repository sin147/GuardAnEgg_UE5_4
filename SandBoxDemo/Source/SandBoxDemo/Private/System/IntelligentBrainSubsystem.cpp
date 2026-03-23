// Fill out your copyright notice in the Description page of Project Settings.

#include "System/IntelligentBrainSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

// 示例：如何在蓝图中使用这个子系统
// 1. 首先获取子系统：Get Game Instance -> Get Subsystem (IntelligentBrainSubsystem)
// 2. 设置API密钥：Set API Key (Provider: Kimi, APIKey: "your-api-key")
// 3. 提问：Thinking (Question: "你好，请介绍一下虚幻引擎", Provider: Kimi)

void UIntelligentBrainSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// 在子系统初始化时自动设置硬编码的Kimi API密钥
	FString DefaultKimiAPIKey = TEXT("sk-l3ew9N05hYjWKrBhkGnBtKX1IbRvDUPKNS95pCsqPvIwLEZ8");
	APIKeys.Add(EAIProvider::Kimi, DefaultKimiAPIKey);
	
	// 在子系统初始化时自动设置硬编码的豆包 API密钥
	FString DefaultDoubaoAPIKey = TEXT("b295a3eb-f46d-466b-b6b6-07415589ccd7");
	APIKeys.Add(EAIProvider::Doubao, DefaultDoubaoAPIKey);
	
	// 在子系统初始化时自动设置硬编码的混元 API密钥
	FString DefaultHunyuanAPIKey = TEXT("sk-FeMlfKmXJK5fJgvbBTNsi26Pkph6CxoHEEsishF1ekjq9sYK");
	APIKeys.Add(EAIProvider::Hunyuan, DefaultHunyuanAPIKey);
	
	UE_LOG(LogTemp, Log, TEXT("IntelligentBrainSubsystem 初始化完成，已设置默认Kimi、豆包和混元 API密钥"));
}

void UIntelligentBrainSubsystem::Thinking(const FString& Question, EAIProvider Provider, FOnAIResponseComplete OnComplete)
{
	// 异步实现：使用HTTP回调
	switch (Provider)
	{
	case EAIProvider::Kimi:
		CallKimiAPI(Question, OnComplete);
		break;
	case EAIProvider::Doubao:
		CallDoubaoAPI(Question, OnComplete);
		break;
	case EAIProvider::Hunyuan:
		CallHunyuanAPI(Question, OnComplete);
		break;
	default:
		OnComplete.ExecuteIfBound(TEXT("不支持的AI提供商类型"));
		break;
	}
}

void UIntelligentBrainSubsystem::SetAPIKey(EAIProvider Provider, const FString& APIKey)
{
	APIKeys.Add(Provider, APIKey);
	UE_LOG(LogTemp, Log, TEXT("设置 %s API密钥成功"), *UEnum::GetValueAsString(Provider));
}

void UIntelligentBrainSubsystem::SetBaseURL(EAIProvider Provider, const FString& BaseURL)
{
	BaseURLs.Add(Provider, BaseURL);
	UE_LOG(LogTemp, Log, TEXT("设置 %s 基础URL: %s"), *UEnum::GetValueAsString(Provider), *BaseURL);
}

FString UIntelligentBrainSubsystem::QuickTest(const FString& APIKey, EAIProvider Provider)
{
	// 设置API密钥
	SetAPIKey(Provider, APIKey);
	
	// 测试问题
	FString TestQuestion = TEXT("请用一句话介绍你自己");
	
	UE_LOG(LogTemp, Log, TEXT("开始快速测试 %s API"), *UEnum::GetValueAsString(Provider));
	
	// 警告：QuickTest方法需要重新实现为异步方式
	UE_LOG(LogTemp, Warning, TEXT("QuickTest方法需要重新实现为异步方式，当前返回空字符串"));
	
	return TEXT("QuickTest方法需要重新实现为异步方式");
}











void UIntelligentBrainSubsystem::CallKimiAPI(const FString& Question, FOnAIResponseComplete OnComplete)
{
	FString APIKey = APIKeys.FindRef(EAIProvider::Kimi);
	FString BaseURL = BaseURLs.FindRef(EAIProvider::Kimi);
	
	if (APIKey.IsEmpty())
	{
		// 硬编码的默认Kimi API密钥
		APIKey = TEXT("sk-l3ew9N05hYjWKrBhkGnBtKX1IbRvDUPKNS95pCsqPvIwLEZ8");
		UE_LOG(LogTemp, Log, TEXT("使用硬编码的Kimi API密钥"));
	}
	
	if (BaseURL.IsEmpty())
	{
		BaseURL = TEXT("https://api.moonshot.cn/v1/chat/completions");
	}
	
	// 创建HTTP请求
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(BaseURL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey));
	// 设置请求超时（30秒）
	Request->SetTimeout(30);
	
	// 构建请求体 - 使用最快的模型
	TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
	RequestObj->SetStringField("model", "kimi-k2.5-lite");
	
	TArray<TSharedPtr<FJsonValue>> Messages;
	TSharedPtr<FJsonObject> MessageObj = MakeShareable(new FJsonObject);
	MessageObj->SetStringField("role", "user");
	MessageObj->SetStringField("content", Question);
	Messages.Add(MakeShareable(new FJsonValueObject(MessageObj)));
	
	RequestObj->SetArrayField("messages", Messages);
	// 大幅限制输出长度以实现快速响应
	RequestObj->SetNumberField("max_tokens", 128);
	// 降低温度参数，减少生成复杂度
	RequestObj->SetNumberField("temperature", 0.3);
	// 启用流式响应（如果支持）
	RequestObj->SetBoolField("stream", false);
	
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer);
	
	Request->SetContentAsString(RequestBody);
	
	UE_LOG(LogTemp, Log, TEXT("异步发送Kimi API请求: %s"), *Question);
	
	// 设置回调函数
	Request->OnProcessRequestComplete().BindLambda([OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (bWasSuccessful && Response.IsValid())
		{
			FString ResponseBody = Response->GetContentAsString();
			UE_LOG(LogTemp, Log, TEXT("Kimi API异步响应: %s"), *ResponseBody);
			
			// 解析JSON响应
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
			
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (JsonObject->HasField(TEXT("choices")) && JsonObject->GetArrayField(TEXT("choices")).Num() > 0)
				{
					TSharedPtr<FJsonObject> Choice = JsonObject->GetArrayField(TEXT("choices"))[0]->AsObject();
					if (Choice->HasField(TEXT("message")))
					{
						TSharedPtr<FJsonObject> Message = Choice->GetObjectField(TEXT("message"));
						if (Message->HasField(TEXT("content")))
						{
							FString Answer = Message->GetStringField(TEXT("content"));
							UE_LOG(LogTemp, Log, TEXT("Kimi API异步回答: %s"), *Answer);
							OnComplete.ExecuteIfBound(Answer);
							return;
						}
					}
				}
				else if (JsonObject->HasField(TEXT("error")))
				{
					TSharedPtr<FJsonObject> Error = JsonObject->GetObjectField(TEXT("error"));
					FString ErrorMessage = FString::Printf(TEXT("API错误: %s"), *Error->GetStringField(TEXT("message")));
					UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
					OnComplete.ExecuteIfBound(ErrorMessage);
					return;
				}
			}
			
			UE_LOG(LogTemp, Error, TEXT("解析Kimi API异步响应失败"));
			OnComplete.ExecuteIfBound(TEXT("解析响应失败"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Kimi API异步请求失败"));
			OnComplete.ExecuteIfBound(TEXT("网络请求失败"));
		}
	});
	
	// 异步执行请求（不会阻塞游戏线程）
	Request->ProcessRequest();
}





void UIntelligentBrainSubsystem::CallDoubaoAPI(const FString& Question, FOnAIResponseComplete OnComplete)
{
	FString APIKey = APIKeys.FindRef(EAIProvider::Doubao);
	FString BaseURL = BaseURLs.FindRef(EAIProvider::Doubao);
	
	if (APIKey.IsEmpty())
	{
		OnComplete.ExecuteIfBound(TEXT("请先设置豆包 API密钥"));
		return;
	}
	
	if (BaseURL.IsEmpty())
	{
		BaseURL = TEXT("https://ark.cn-beijing.volces.com/api/v3/responses");
	}
	
	// 创建HTTP请求
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(BaseURL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey));
	// 设置请求超时（30秒）
	Request->SetTimeout(30);
	
	// 构建请求体 - 使用最快的模型
	TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
	RequestObj->SetStringField("model", "doubao-seed-2-0-lite");
	
	TArray<TSharedPtr<FJsonValue>> Messages;
	TSharedPtr<FJsonObject> InputObj = MakeShareable(new FJsonObject);
	InputObj->SetStringField("role", "user");
	InputObj->SetStringField("content", Question);
	Messages.Add(MakeShareable(new FJsonValueObject(InputObj)));
	
	RequestObj->SetArrayField("input", Messages);
	// 大幅限制输出长度以实现快速响应
	RequestObj->SetNumberField("max_tokens", 128);
	// 降低温度参数，减少生成复杂度
	RequestObj->SetNumberField("temperature", 0.3);
	// 启用流式响应（如果支持）
	RequestObj->SetBoolField("stream", false);
	
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer);
	
	Request->SetContentAsString(RequestBody);
	
	UE_LOG(LogTemp, Log, TEXT("异步发送豆包 API请求: %s"), *Question);
	
	// 设置回调函数
	Request->OnProcessRequestComplete().BindLambda([OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (bWasSuccessful && Response.IsValid())
		{
			FString ResponseBody = Response->GetContentAsString();
			UE_LOG(LogTemp, Log, TEXT("豆包 API异步响应: %s"), *ResponseBody);
			
			// 解析JSON响应
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
			
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (JsonObject->HasField(TEXT("choices")) && JsonObject->GetArrayField(TEXT("choices")).Num() > 0)
				{
					TSharedPtr<FJsonObject> Choice = JsonObject->GetArrayField(TEXT("choices"))[0]->AsObject();
					if (Choice->HasField(TEXT("message")))
					{
						TSharedPtr<FJsonObject> Message = Choice->GetObjectField(TEXT("message"));
						if (Message->HasField(TEXT("content")))
						{
							FString Answer = Message->GetStringField(TEXT("content"));
							UE_LOG(LogTemp, Log, TEXT("豆包 API异步回答: %s"), *Answer);
							OnComplete.ExecuteIfBound(Answer);
							return;
						}
					}
				}
				else if (JsonObject->HasField(TEXT("error")))
				{
					TSharedPtr<FJsonObject> Error = JsonObject->GetObjectField(TEXT("error"));
					FString ErrorMessage = FString::Printf(TEXT("API错误: %s"), *Error->GetStringField(TEXT("message")));
					UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
					OnComplete.ExecuteIfBound(ErrorMessage);
					return;
				}
			}
			
			UE_LOG(LogTemp, Error, TEXT("解析豆包 API异步响应失败"));
			OnComplete.ExecuteIfBound(TEXT("解析响应失败"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("豆包 API异步请求失败"));
			OnComplete.ExecuteIfBound(TEXT("网络请求失败"));
		}
	});
	
	// 异步执行请求
	Request->ProcessRequest();
}

void UIntelligentBrainSubsystem::CallHunyuanAPI(const FString& Question, FOnAIResponseComplete OnComplete)
{
	FString APIKey = APIKeys.FindRef(EAIProvider::Hunyuan);
	FString BaseURL = BaseURLs.FindRef(EAIProvider::Hunyuan);
	
	if (APIKey.IsEmpty())
	{
		// 硬编码的默认混元 API密钥
		APIKey = TEXT("sk-FeMlfKmXJK5fJgvbBTNsi26Pkph6CxoHEEsishF1ekjq9sYK");
		UE_LOG(LogTemp, Log, TEXT("使用硬编码的混元 API密钥"));
	}
	
	if (BaseURL.IsEmpty())
	{
		BaseURL = TEXT("https://api.hunyuan.cloud.tencent.com/v1/chat/completions");
	}
	
	// 创建HTTP请求
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(BaseURL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("Authorization", FString::Printf(TEXT("Bearer %s"), *APIKey));
	// 设置请求超时（30秒）
	Request->SetTimeout(30);
	
	// 构建请求体 - 使用最快的模型版本
	TSharedPtr<FJsonObject> RequestObj = MakeShareable(new FJsonObject);
	RequestObj->SetStringField("model", "hunyuan-lite");
	
	TArray<TSharedPtr<FJsonValue>> Messages;
	TSharedPtr<FJsonObject> MessageObj = MakeShareable(new FJsonObject);
	MessageObj->SetStringField("role", "user");
	MessageObj->SetStringField("content", Question);
	Messages.Add(MakeShareable(new FJsonValueObject(MessageObj)));
	
	RequestObj->SetArrayField("messages", Messages);
    // 大幅限制输出长度以实现快速响应
    RequestObj->SetNumberField("max_tokens", 64);
    // 降低温度参数，减少生成复杂度
    RequestObj->SetNumberField("temperature", 0.001);
    // 启用流式响应（如果支持）
    RequestObj->SetBoolField("stream", false);
	
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer);
	
	Request->SetContentAsString(RequestBody);
	
	UE_LOG(LogTemp, Log, TEXT("异步发送混元 API请求: %s"), *Question);
	
	// 设置回调函数
	Request->OnProcessRequestComplete().BindLambda([OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (bWasSuccessful)
		{
			if (Response.IsValid())
			{
				FString ResponseBody = Response->GetContentAsString();
				UE_LOG(LogTemp, Log, TEXT("混元 API异步响应: %s"), *ResponseBody);
				
				// 解析JSON响应
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
				
				if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
				{
					if (JsonObject->HasField(TEXT("choices")) && JsonObject->GetArrayField(TEXT("choices")).Num() > 0)
					{
						TSharedPtr<FJsonObject> Choice = JsonObject->GetArrayField(TEXT("choices"))[0]->AsObject();
						if (Choice->HasField(TEXT("message")))
						{
							TSharedPtr<FJsonObject> Message = Choice->GetObjectField(TEXT("message"));
							if (Message->HasField(TEXT("content")))
							{
								FString Answer = Message->GetStringField(TEXT("content"));
								UE_LOG(LogTemp, Log, TEXT("混元 API异步回答: %s"), *Answer);
								OnComplete.ExecuteIfBound(Answer);
								return;
							}
						}
					}
					else if (JsonObject->HasField(TEXT("error")))
					{
						TSharedPtr<FJsonObject> Error = JsonObject->GetObjectField(TEXT("error"));
						FString ErrorMessage = FString::Printf(TEXT("API错误: %s"), *Error->GetStringField(TEXT("message")));
						UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
						OnComplete.ExecuteIfBound(ErrorMessage);
						return;
					}
				}
				
				UE_LOG(LogTemp, Error, TEXT("解析混元 API异步响应失败"));
				OnComplete.ExecuteIfBound(TEXT("解析响应失败"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("混元 API响应无效"));
				OnComplete.ExecuteIfBound(TEXT("响应无效"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("混元 API异步请求失败"));
			OnComplete.ExecuteIfBound(TEXT("网络请求失败"));
		}
	});
	
	// 异步执行请求
	Request->ProcessRequest();
}