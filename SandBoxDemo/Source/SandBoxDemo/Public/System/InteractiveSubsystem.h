// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NetworkProxyActor/InteractiveProxyActor.h"
#include "Interface/Interact.h"
#include "InteractiveSubsystem.generated.h"

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
	//被动交互对象
	UPROPERTY(BlueprintReadOnly)
	TArray<FGuid> PassiveInteractiveActorGUIDs;
	//主动交互对象
	UPROPERTY(BlueprintReadOnly)
	TArray<FGuid> ActiveInteractiveActorGUIDs;
	//当前交互状态


public:
	FCharacterInteractiveInfo() = default;
	FCharacterInteractiveInfo(TObjectPtr<ACharacter> Character);
	//添加交互对象
	bool AddInteractiveActor(EInteractiveType InInteractiveType, FGuid InGUID)
	{
		switch (InInteractiveType)
		{
		case IT_None:
			break;
		case IT_Active:
			if (!ActiveInteractiveActorGUIDs.Contains(InGUID))
			{
				return ActiveInteractiveActorGUIDs.Add(InGUID) >= 0;
			}
			break;
		case IT_Passive:
			if (!PassiveInteractiveActorGUIDs.Contains(InGUID))
			{
				return PassiveInteractiveActorGUIDs.Add(InGUID) >= 0;
			}
			break;
		default:
			break;
		}
		return false;
	}
	//移除交互对象
	bool RemoveInteractiveActor(FGuid InGUID)
	{
		if (PassiveInteractiveActorGUIDs.Contains(InGUID))
		{
			return PassiveInteractiveActorGUIDs.Remove(InGUID)>=0;
		}
		else if (ActiveInteractiveActorGUIDs.Contains(InGUID))
		{
			return ActiveInteractiveActorGUIDs.Remove(InGUID) >= 0;
		}
		return false;
	}
	//获取交互对象列表
	TArray<FGuid> GetInteractiveActorGUIDs(EInteractiveType InInteractiveType) const
	{
		switch (InInteractiveType)
		{
		case IT_None:
			break;
		case IT_Active:
			return ActiveInteractiveActorGUIDs;
		case IT_Passive:
			return PassiveInteractiveActorGUIDs;
		default:
			break;
		}
		return TArray<FGuid>();
	}
};

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UInteractiveSubsystem : public UGameInstanceSubsystem,public FTickableGameObject
{
	GENERATED_BODY()
private:
	friend class AInteractiveProxyActor;
	//网络代理
	TObjectPtr<AInteractiveProxyActor> InteractiveProxy;
protected:
	//交互角色信息
	UPROPERTY(BlueprintReadOnly)
	TMap<ACharacter*,FCharacterInteractiveInfo> CharacterInteractiveInfos;
	//交互对象组
	TArray<IInteract*> InteractiveActors;
public:
	//请求交互
	void RequestInteract(ACharacter* InCharacter);
	//填充交互对象
	bool PaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter, EInteractiveType InInteractiveType, FGuid InInteractiveActorGUID);
	//填充交互对象
	bool UnPaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter, FGuid InInteractiveActorGUID);
	UFUNCTION(BlueprintCallable, Category = "Interactive")
	void SpawnInteractiveActor(TSubclassOf<AActor> ActorClass,FVector InLocation, FRotator InRotation);

	//获取交互对象通过GUID
	template<typename T=IInteract>
	T* GetInteractiveActorByGUID(FGuid InGUID) const
	{
		for (IInteract* Interactable : InteractiveActors)
		{
			// 一步 Cast 到 T*，同时检查 Interactable 是否有效、是否是目标类型
			AActor* TargetActor = Cast<AActor>(Interactable);
			if (TargetActor && TargetActor->GetActorGuid() == InGUID)
			{
				return Cast<T>(TargetActor);
			}
		}
		return nullptr;
	}
	//通过GUID获取交互对象们
	TArray<IInteract*> GetInteractiveActorsByGUIDs(TArray<FGuid> InGUIDs ) const;
	//销毁交互对象
	void DestoryInteractiveActorByGuid(FGuid Guid);
	//初始化
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	//CanInteract过滤交互对象列表
	bool CanInteractFilter(ACharacter* InCharacter) const;

	TArray<FGuid> FilterInteractiveActor(const TArray<FGuid> InInteractiveActorGUID) const;
	void Server_Interact(ACharacter* InCharacter);
	void Multicast_Interact(AActor*InActor, ACharacter* InCharacter);
	void Multicast_OnSpawnInteractiveActor(AActor* NewInteractiveActor);
	void Server_SpawnInteractiveActor(TSubclassOf<AActor> ActorClass, FVector InLocation, FRotator InRotation);

	//设置代理Actor
	void SetInteractiveProxy(AInteractiveProxyActor* InProxy);
public:
	// ========== FTickableGameObject接口（必须实现） ==========
// 帧更新逻辑（核心Tick函数）
	virtual void Tick(float DeltaTime) override;
	// 获取Tick的优先级（值越小优先级越高，默认0即可）
	virtual ETickableTickType GetTickableTickType() const override;
	// 获取当前对象的World（GameInstanceSubsystem关联GameInstance的World）
	virtual UWorld* GetTickableGameObjectWorld() const override;
	TStatId GetStatId() const override;
};
