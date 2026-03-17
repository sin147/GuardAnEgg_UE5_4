// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AttributeSubsystem.generated.h"
//角色属性
UENUM(BlueprintType)
enum EAttribute : uint8
{
	//无
	CA_None UMETA(DisplayName = "无"),

	//血量
	HP UMETA(DisplayName = "血量"),

	//行走速度
	WalkSpeed UMETA(DisplayName = "行走速度（快速移动速度）"),

	//飞行速度
	FlySpeed UMETA(DisplayName = "飞行速度（快速移动速度）"),

	//游泳速度
	SwimmingSpeed UMETA(DisplayName = "游泳速度（快速移动速度）"),

	//左右转向速度
	YawRotatorSpeed UMETA(DisplayName = "左右转向速度"),

	//上下旋转速度 
	PitchRotatorSpeed UMETA(DisplayName = "上下转向速度"),

	//耐力
	EnduranceValue UMETA(DisplayName = "耐力"),

};
//角色属性值枚举
UENUM(BlueprintType)
enum ECAVType :uint8
{
	//网络事件测试
	CAVT_Max UMETA(DisplayName = "最大值"),
	CAVT_Currently UMETA(DisplayName = "当前值"),
	CAVT_Min UMETA(DisplayName = "最小值"),

};
/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UAttributeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	friend class AAttributeProxyActor;
private:
	//代理
	TObjectPtr<AAttributeProxyActor> AttributeProxy;

protected:
	//设置代理
	void SetAttributeProxy(AAttributeProxyActor*InProxyActor);

	//服务器设置值
	void Server_SetAttributeByEnum(AActor* InActor, EAttribute InAttribute, float InValue, ECAVType InAttributeValueType);
	//多播设置值
	void Multicast_SetAttributeByEnum(AActor* InActor, EAttribute InAttribute,float InValue, ECAVType InAttributeValueType);

public:
	//获取值
	UFUNCTION(BlueprintCallable)
	float GetAttributeByEnum(AActor* InActor,EAttribute InAttribute, ECAVType InAttributeValueType = ECAVType::CAVT_Currently);

	//设置值
	UFUNCTION(BlueprintCallable)
	void SetAttributeByEnum(AActor* InActor,EAttribute InAttribute, float Value, ECAVType InAttributeValueType = ECAVType::CAVT_Currently);


};
