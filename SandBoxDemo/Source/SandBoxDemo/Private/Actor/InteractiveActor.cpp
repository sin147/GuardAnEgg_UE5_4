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
			InteractiveSubsystem->PaddingInteractiveActor(Character,this->GetActorGuid());
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
			InteractiveSubsystem->UnPaddingInteractiveActor(Character,this->GetActorGuid());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InteractiveSubsystem is null"));
		}
	}

}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveActor::Interact(ACharacter* InCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("%s:Interact With %s"), * GetName(),*InCharacter->GetActorNameOrLabel());
}

bool AInteractiveActor::CanInteract(ACharacter* InCharacter)
{
	
	//Todo:添加交互条件
	return IInteract::CanInteract(InCharacter);
}

