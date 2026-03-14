// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractiveActor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "System/InteractiveSubsystem.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//创建触发框组件
	TriggerBox = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	TriggerBox->SetupAttachment(RootComponent);
	//交互姿态机
	InteractiveStateMachine = CreateDefaultSubobject<UStateMachineBase>( TEXT("InteractiveStateMachine"));
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	//绑定触发事件
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::OnTriggerBoxOverlapBegin);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::OnTriggerBoxOverlapEnd);
	}
	
}

void AInteractiveActor::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//将角色和交互对象的注册到交互系统中
	if (Other && Other->IsA<ACharacter>())
	{
		ACharacter* Character = Cast<ACharacter>(Other);
		UInteractiveSubsystem* InteractiveSubsystem = GetGameInstance()->GetSubsystem<UInteractiveSubsystem>();
		if (InteractiveSubsystem)
		{
			if (InteractiveSubsystem->PaddingInteractiveActor(Character, this->GetInteractiveType(), this->GetActorGuid())&&( InteractiveStateMachine ->GetCurrentlyState() == S_None || InteractiveStateMachine->GetCurrentlyState() == S_InteractiveActor_Break))
			{
				EnterState(S_InteractiveActor_Pre);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InteractiveSubsystem is null"));
		}
	}
}

void AInteractiveActor::OnTriggerBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//将角色和交互对象从交互系统中移除
	if(Other && Other->IsA<ACharacter>())
	{
		ACharacter* Character = Cast<ACharacter>(Other);
		UInteractiveSubsystem* InteractiveSubsystem = GetGameInstance()->GetSubsystem<UInteractiveSubsystem>();
		if (InteractiveSubsystem)
		{
			if (InteractiveSubsystem->UnPaddingInteractiveActor(Character, this->GetActorGuid()))
			{
				EnterState(S_InteractiveActor_Break);
			}
			InteractiveCharacters.Remove(Character);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InteractiveSubsystem is null"));
		}
	}

}

void AInteractiveActor::PreInteractImp_Implementation(float InCurrentlyStateTime, float InTotalDurationTime)
{
	UE_LOG(LogTemp, Log, TEXT("%s:PreInteractImp"), *GetName());
}

void AInteractiveActor::StartInteractImp_Implementation(float InCurrentlyStateTime, float InTotalDurationTime)
{
	UE_LOG(LogTemp, Log, TEXT("%s:StartInteractImp"), *GetName());
}

void AInteractiveActor::InteracttingImp_Implementation(float InCurrentlyStateTime, float InTotalDurationTime)
{
	UE_LOG(LogTemp, Log, TEXT("%s:InteracttingImp"), *GetName());
}

void AInteractiveActor::InteractOverImp_Implementation(float InCurrentlyStateTime, float InTotalDurationTime)
{
	UE_LOG(LogTemp, Log, TEXT("%s:InteractOverImp"), *GetName());
}

void AInteractiveActor::InteractBreakImp_Implementation(float InCurrentlyStateTime, float InTotalDurationTime)
{
	UE_LOG(LogTemp, Log, TEXT("%s:InteractBreakImp"), *GetName());
}

void AInteractiveActor::PreInteract(float InCurrentlyStateTime, float InTotalDurationTime)
{
	PreInteractImp(InCurrentlyStateTime, InTotalDurationTime);
	if (InteractTime >= PreInteractDuration&&InteractiveType==IT_Passive)
	{
		EnterState(EState::S_InteractiveActor_Interacting);
	} 
}

void AInteractiveActor::StartInteract(float InCurrentlyStateTime, float InTotalDurationTime)
{
	StartInteractImp(InCurrentlyStateTime, InTotalDurationTime);
	if (InteractTime >= StartInteractDuration)
	{
		EnterState(EState::S_InteractiveActor_Interacting);
	}
}

void AInteractiveActor::Interactting(float InCurrentlyStateTime, float InTotalDurationTime)
{
	InteracttingImp(InCurrentlyStateTime, InTotalDurationTime);
	if (InteractTime >= InteractDuration)
	{
		EnterState(EState::S_InteractiveActor_Finish);
	}
}

void AInteractiveActor::InteractOver(float InCurrentlyStateTime, float InTotalDurationTime)
{
	InteractOverImp(InCurrentlyStateTime, InTotalDurationTime);
	if (InteractTime >= InteractOverDuration)
	{
		EnterState(EState::S_None);
		InteractiveCharacters.Empty();
	}
}

void AInteractiveActor::InteractBreak(float InCurrentlyStateTime, float InTotalDurationTime)
{
	InteractBreakImp(InCurrentlyStateTime, InTotalDurationTime);
	if (InteractTime >= InteractBreakDuration)
	{
		EnterState(EState::S_None);
	}
}

TArray<ACharacter*> AInteractiveActor::GetInteractiveCharacters()
{
	return InteractiveCharacters;
}

void AInteractiveActor::EnterState(EState InState)
{
	InteractTime = 0.f;
	InteractiveStateMachine->EnterState(InState);
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (InteractiveStateMachine->GetCurrentlyState())
	{
	case S_None:
		break;
	case S_InteractiveActor_Pre:
		PreInteract(InteractTime,PreInteractDuration);
		break;
	case S_InteractiveActor_Start:
		StartInteract(InteractTime,StartInteractDuration);
		break;
	case S_InteractiveActor_Interacting:
		Interactting(InteractTime,InteractDuration);
		break;
	case S_InteractiveActor_Break:
		InteractBreak(InteractTime,InteractBreakDuration);
		break;
	case S_InteractiveActor_Finish:
		InteractOver(InteractTime,InteractOverDuration);
		break;
	default:
		break;
	}
	if (InteractiveStateMachine->GetCurrentlyState() != S_None)
	{
		InteractTime += DeltaTime;
	}
}

void AInteractiveActor::Interact(ACharacter* InCharacter)
{
	InteractiveCharacters.Add(InCharacter);
	EnterState(EState::S_InteractiveActor_Start);
	UE_LOG(LogTemp, Log, TEXT("%s:Interact With %s"), * GetName(),*InCharacter->GetActorNameOrLabel());
}

bool AInteractiveActor::CanInteract(ACharacter* InCharacter)
{
	//Todo:添加交互条件
	return IInteract::CanInteract(InCharacter)&&IsValid(InCharacter)&&(InteractiveCharacters.Find(InCharacter) == INDEX_NONE);


}
void AInteractiveActor::InteractBreak(ACharacter* InCharacter)
{
	EnterState(EState::S_InteractiveActor_Break);
}
TEnumAsByte<EState> AInteractiveActor::GetCurrentlyState()
{
	if (InteractiveStateMachine)
	{
		return InteractiveStateMachine->GetCurrentlyState();
	}
	return S_None;
}
