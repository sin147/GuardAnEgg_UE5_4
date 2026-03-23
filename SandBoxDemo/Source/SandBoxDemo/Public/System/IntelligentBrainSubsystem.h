// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IntelligentBrainSubsystem.generated.h"

/**
 * AI大模型类型枚举
 */
UENUM(BlueprintType)
enum class EAIProvider : uint8
{
	Kimi UMETA(DisplayName = "Kimi"),
	Doubao UMETA(DisplayName = "Doubao"),
	Hunyuan UMETA(DisplayName = "Hunyuan")
};

/**
 * AI响应完成委托
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAIResponseComplete, const FString&, Response);

/**
 * AI大模型智能大脑子系统
 * 
 * 使用说明：
 * 1. 在蓝图中获取子系统：Get Game Instance -> Get Subsystem (IntelligentBrainSubsystem)
 * 2. 设置API密钥：Set API Key (Provider: Kimi/Doubao/Hunyuan, APIKey: "your-api-key")
 * 3. 提问：Thinking (Question: "你的问题", Provider: Kimi/Doubao/Hunyuan, OnComplete: 回调函数)
 * 
 * 支持的AI提供商：
 * - Kimi (Moonshot AI): https://platform.moonshot.cn/
 * - Doubao (豆包): https://www.doubao.com/
 * - Hunyuan (混元): https://hunyuan.tencent.com/
 */
UCLASS()
class SANDBOXDEMO_API UIntelligentBrainSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	/**
	 * 子系统初始化方法
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	/**
	 * 异步向AI大模型提问
	 * @param Question 问题内容
	 * @param Provider AI提供商类型
	 * @param OnComplete 完成回调
	 */
	UFUNCTION(BlueprintCallable, Category = "AI Brain")
	void Thinking(const FString& Question, EAIProvider Provider, FOnAIResponseComplete OnComplete);
	
	/**
	 * 设置API密钥
	 * @param Provider AI提供商类型
	 * @param APIKey API密钥
	 */
	UFUNCTION(BlueprintCallable, Category = "AI Brain")
	void SetAPIKey(EAIProvider Provider, const FString& APIKey);
	
	/**
	 * 设置API基础URL
	 * @param Provider AI提供商类型
	 * @param BaseURL API基础URL
	 */
	UFUNCTION(BlueprintCallable, Category = "AI Brain")
	void SetBaseURL(EAIProvider Provider, const FString& BaseURL);
	
	/**
	 * 快速测试AI功能（示例方法）
	 * @param APIKey API密钥
	 * @param Provider AI提供商类型
	 * @return 测试结果
	 */
	UFUNCTION(BlueprintCallable, Category = "AI Brain|Test")
	FString QuickTest(const FString& APIKey, EAIProvider Provider = EAIProvider::Kimi);
	
protected:

	
	/**
	 * 调用Kimi API
	 * @param Question 问题内容
	 * @param OnComplete 完成回调
	 */
	void CallKimiAPI(const FString& Question, FOnAIResponseComplete OnComplete);
	

	
	/**
	 * 调用豆包API
	 * @param Question 问题内容
	 * @param OnComplete 完成回调
	 */
	void CallDoubaoAPI(const FString& Question, FOnAIResponseComplete OnComplete);
	
	/**
	 * 调用混元API
	 * @param Question 问题内容
	 * @param OnComplete 完成回调
	 */
	void CallHunyuanAPI(const FString& Question, FOnAIResponseComplete OnComplete);
	
private:
	/** API密钥配置 */
	UPROPERTY()
	TMap<EAIProvider, FString> APIKeys;
	
	/** API基础URL配置 */
	UPROPERTY()
	TMap<EAIProvider, FString> BaseURLs;
};