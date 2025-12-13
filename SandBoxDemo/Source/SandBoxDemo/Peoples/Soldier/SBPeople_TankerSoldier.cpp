// Fill out your copyright notice in the Description page of Project Settings.


#include "Peoples/Soldier/SBPeople_TankerSoldier.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASBPeople_TankerSoldier::ASBPeople_TankerSoldier()
{
	//设置技能碰撞框
	SkillCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SkillCollisionComp"));
	SkillCollisionComp->SetupAttachment(RootComponent);
	SkillCollisionComp->SetSphereRadius(SkillRadius);
	DefensePower = 20;
}

void ASBPeople_TankerSoldier::BeginPlay()
{
	Super::BeginPlay();
	//绑定技能碰撞框事件
	if (SkillCollisionComp)
	{
		SkillCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASBPeople_TankerSoldier::OnSkillCollisionBeginOverlap);
		SkillCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ASBPeople_TankerSoldier::OnSkillCollisionEndOverlap);
		SkillCollisionComp->MoveIgnoreActors.Add(this);
	}
}

void ASBPeople_TankerSoldier::ApplySkillToPeople(ASandBoxPeople* InPeople)
{

	NiagaraComponents.Add(InPeople->GetName(), UNiagaraFunctionLibrary::SpawnSystemAttached(NSDefense, InPeople->GetRootComponent(), "None", FVector(0, 0, 30), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, false));
	InPeople->SetDefensePower(InPeople->GetDefensePower() + DefensePower);

	UE_LOG(LogTemp, Log, TEXT("应用到友军"));
}



void ASBPeople_TankerSoldier::OnSkillCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		//判断是否为友军
		ASandBoxPeople* OtherPeople = Cast<ASandBoxPeople>(OtherActor);
		if (OtherPeople && OtherPeople != this && IsFriend(OtherPeople)&& !NiagaraComponents.Find(OtherActor->GetName()))
		{
			//为友军时，增加其护甲值
			//SkillCollisionComp->MoveIgnoreActors.Add(OtherPeople);
			ApplySkillToPeople(OtherPeople);
		}

		if (OtherPeople && OtherPeople != this && IsEnemy(OtherPeople))
		{
			//忽略敌军
			SkillCollisionComp->MoveIgnoreActors.Add(OtherPeople);

		}
	
}

void ASBPeople_TankerSoldier::OnSkillCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	ASandBoxPeople* OtherPeople = Cast<ASandBoxPeople>(OtherActor);
	//移除对应的Niagara组件

	if (OtherPeople&&IsFriend(OtherPeople))
	{
		UE_LOG(LogTemp, Log, TEXT("移除友军效果"));
		if (NiagaraComponents.Find(OtherPeople->GetName()))
		{
			(*NiagaraComponents.Find(OtherPeople->GetName()))->DestroyComponent();
			NiagaraComponents.Remove(OtherPeople->GetName());
			OtherPeople->SetDefensePower(OtherPeople->GetDefensePower() - DefensePower);
		}
	}
	
}
