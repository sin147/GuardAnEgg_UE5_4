// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GlobalEnums.h"
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
	TEnumAsByte<EState> CurrentlyState= S_None;
protected:
	//当进入状态
	virtual void OnEnterState(EState InState);
	//当退出状态
	virtual void OnExitState(EState InState);
	//可进入状态
	virtual bool CanEnterStates(EState InState);

public:	
	//进入状态
	void EnterState(EState InState)
	{
		if (!CanEnterStates(InState))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Cannot Enter State:%d"), InState);
			return;
		}
		EState OldState = CurrentlyState;
		EState NewState = InState;
		CurrentlyState= NewState;
		OnExitState(OldState);
		OnEnterState(NewState);
	}

	//获得当前状态
	TEnumAsByte<EState> GetCurrentlyState();

};