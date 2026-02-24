// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InteractiveSubsystem.generated.h"

class AInteractiveActor;

UENUM(BlueprintType)
enum EInteractiveState :uint8
{
	IS_None  UMETA(DisplayName = "无交互"),
	IS_Start UMETA(DisplayName = "开始交互"),
	IS_Interacting UMETA(DisplayName = "交互中"),
	IS_Break UMETA(DisplayName = "中断"),
	IS_Finish UMETA(DisplayName = "交互完成"),

};
//过滤类型
UENUM(BlueprintType)
enum EFilterType:uint8
{
	FT_None UMETA(DisplayName = "无过滤"),
	FT_Character UMETA(DisplayName = "角色过滤"),
	FT_Setting UMETA(DisplayName = "设置过滤"),

};

USTRUCT(BlueprintType)
struct FCharacterInteractiveInfo
{
	GENERATED_BODY()
protected:
	//交互角色GUID
	UPROPERTY(BlueprintReadOnly)
	 TObjectPtr<ACharacter> Character;
	//交互对象
	UPROPERTY(BlueprintReadOnly)
	TArray<FGuid> InteractiveActorGUIDs;
	//当前交互状态


public:
	FCharacterInteractiveInfo() = default;
	FCharacterInteractiveInfo(TObjectPtr<ACharacter> Character);
	//添加交互对象
	bool AddInteractiveActor(FGuid InGUID)
	{
		if (!InteractiveActorGUIDs.Contains(InGUID))
		{
			return InteractiveActorGUIDs.Add(InGUID)>=0;
		}
		return false;
	}
	//移除交互对象
	bool RemoveInteractiveActor(FGuid InGUID)
	{
		if (InteractiveActorGUIDs.Contains(InGUID))
		{
			return InteractiveActorGUIDs.Remove(InGUID)>=0;
		}
		return false;
	}
	//获取交互对象列表
	TArray<FGuid> GetInteractiveActorGUIDs() const
	{
		return InteractiveActorGUIDs;
	}
};

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UInteractiveSubsystem : public UGameInstanceSubsystem,public FTickableGameObject
{
	GENERATED_BODY()
protected:
	//交互角色信息
	UPROPERTY(BlueprintReadOnly)
	TMap<ACharacter*,FCharacterInteractiveInfo> CharacterInteractiveInfos;
	//交互对象组
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AInteractiveActor>> InteractiveActors;
public:
	//请求交互
	void RequestInteract(ACharacter* InCharacter);
	//填充交互对象
	bool PaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter, FGuid InInteractiveActorGUID);
	//填充交互对象
	bool UnPaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter, FGuid InInteractiveActorGUID);
	//生成交互对象
	UFUNCTION(Server,Reliable,BlueprintCallable)
	void SpawnInteractiveActor(TSubclassOf<AInteractiveActor>ActorClass,FVector InLocaltion, FRotator InRotation);

	//获取交互对象通过GUID
	AInteractiveActor* GetInteractiveActorByGUID(FGuid InGUID) const;
protected:
	//CanInteract过滤交互对象列表
	bool CanInteractFilter(ACharacter* InCharacter) const;

	TArray<FGuid> FilterInteractiveActor(const TArray<FGuid> InInteractiveActorGUID) const;
	//服务器交互
	UFUNCTION(Server, Reliable)
	void Server_Interact(ACharacter* InCharacter);
	//客户端多播交互
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Interact(ACharacter* InCharacter);
public:
	// ========== FTickableGameObject接口（必须实现） ==========
// 帧更新逻辑（核心Tick函数）
	virtual void Tick(float DeltaTime) override;
	// 获取Tick的优先级（值越小优先级越高，默认0即可）
	virtual ETickableTickType GetTickableTickType() const override;
	// 获取当前对象的World（GameInstanceSubsystem关联GameInstance的World）
	virtual UWorld* GetTickableGameObjectWorld() const override;
	TStatId GetStatId() const override;
protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//当服务器生成交互对象时，通知客户端更新交互对象列表
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnSpawnInteractiveActor(AInteractiveActor* NewInteractiveActor);
};
