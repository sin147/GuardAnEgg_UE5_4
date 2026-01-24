// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineBase.generated.h"

class ASandBoxCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANDBOXDEMO_API UStateMachineBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineBase();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//当进入状态
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;




private:
	//当前状态
	uint8 CurrentlyState=0;
protected:
	//当进入状态
	virtual void OnEnterState(uint8 InState);
	//当退出状态
	virtual void OnExitState(uint8 InState);

public:	
	//进入状态
	template<typename TEnum>
	void EnterState(TEnum InState)
	{
		//检测是否满足
		static_assert(TIsEnum<TEnum>::Value, "Must Be Enum Type");
		//转为uint8存储
		CurrentlyState=static_cast<uint8>(InState);
		OnEnterState(InState);
	}

	//离开状态
	template<typename TEnum>
	void ExitState(TEnum InState)
	{
		//检测是否为枚举
		static_assert(TIsEnum<TEnum>::Value, "Must Be Enum Type");
		//转为uint8储存
		CurrentlyState = 0;
		OnExitState(InState);
	}

	//获得当前状态
	uint8 GetCurrentlyState();

};
