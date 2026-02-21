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
	void AddInteractiveActor(FGuid InGUID)
	{
		if (!InteractiveActorGUIDs.Contains(InGUID))
		{
			InteractiveActorGUIDs.Add(InGUID);
		}
	}
	//移除交互对象
	void RemoveInteractiveActor(FGuid InGUID)
	{
		if (InteractiveActorGUIDs.Contains(InGUID))
		{
			InteractiveActorGUIDs.Remove(InGUID);
		}
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
class UNITYINTERACTIVE_API UInteractiveSubsystem : public UGameInstanceSubsystem
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
	void PaddingInteractiveActor(TObjectPtr<ACharacter> InCharacter, FGuid InInteractiveActorGUID);
	//生成交互对象
	void SpawnInteractiveActor(FVector InLocaltion, FRotator InRotation);

	//获取交互对象通过GUID
	AInteractiveActor* GetInteractiveActorByGUID(FGuid InGUID) const;
protected:
	TArray<FGuid> FilterInteractiveActor(const TArray<FGuid>& InInteractiveActorGUID) const;
	//服务器交互
	UFUNCTION(Server, Reliable)
	void Server_Interact(ACharacter* InCharacter);
	//客户端多播交互
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Interact(ACharacter* InCharacter);

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//当服务器生成交互对象时，通知客户端更新交互对象列表
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnSpawnInteractiveActor(AInteractiveActor* NewInteractiveActor);
};
