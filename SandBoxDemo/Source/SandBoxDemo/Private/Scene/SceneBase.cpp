// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SceneBase.h"
#include "SandBoxCharacter.h"

// Sets default values
ASceneBase::ASceneBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
}

void ASceneBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//判断是否与玩家发生碰撞
	if(OtherActor && OtherActor != this && OtherComp)
	{
		ASandBoxCharacter* Character = Cast<ASandBoxCharacter>(OtherActor);
		if(Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player entered the box!"));
			Character->StartSwim();
		}
	}

}

void ASceneBase::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//判断是否与玩家发生碰撞
	if(OtherActor && OtherActor != this && OtherComp)
	{
		ASandBoxCharacter* Character = Cast<ASandBoxCharacter>(OtherActor);
		if(Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player left the box!"));
			Character->StopSwim();
		}
	}
}

FVector ASceneBase::GetBoxExtent()
{
	return BoxComponent->GetScaledBoxExtent();
}

// Called when the game starts or when spawned
void ASceneBase::BeginPlay()
{
	Super::BeginPlay();
	if(BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASceneBase::OnBoxBeginOverlap);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ASceneBase::OnBoxEndOverlap);
	}
	
}

// Called every frame
void ASceneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

