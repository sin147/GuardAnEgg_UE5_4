// Fill out your copyright notice in the Description page of Project Settings.


#include "State/StateMachineBase.h"

// Sets default values for this component's properties
UStateMachineBase::UStateMachineBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateMachineBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStateMachineBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateMachineBase::OnEnterState(EState InState)
{
	//进入状态后逻辑
	//UE_LOG(LogTemp, Log, TEXT("%s EnterState State:%d"), *GetOwner()->GetActorNameOrLabel(), CurrentlyState);
}

void UStateMachineBase::OnExitState(EState InState)
{
	//退出状态后逻辑
	//UE_LOG(LogTemp, Log, TEXT("%s ExitState State:%d"), *GetOwner()->GetActorNameOrLabel(), CurrentlyState);
}

bool UStateMachineBase::CanEnterStates(EState InState)
{
	return true;
}

void UStateMachineBase::EnterState(EState InState)
{
	if (!CanEnterStates(InState))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Cannot Enter State:%d"), InState);
		return;
	}
	EState OldState = CurrentlyState;
	EState NewState = InState;
	CurrentlyState = NewState;
	OnExitState(OldState);
	OnEnterState(NewState);
}

TEnumAsByte<EState> UStateMachineBase::GetCurrentlyState()
{
	return CurrentlyState;
}

