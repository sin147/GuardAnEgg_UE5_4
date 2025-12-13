// Fill out your copyright notice in the Description page of Project Settings.


#include "Peoples/Soldier/SBPeople_FarSoldier.h"
#include "../../Props/SandBox_Prop.h"

void ASBPeople_FarSoldier::AttackEnemy()
{
	//初始化攻击目标位置
	FVector AttackLocation;
	if (IsValidPeople(CurrentlyAttackingEnemy))
	{
		AttackLocation = CurrentlyAttackingEnemy->GetActorLocation();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Not Enemy"));
		return;
	}
	//道具生成Transform
	FTransform SpawnTransform(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackLocation), GetActorLocation()+FVector(0,0,30), FVector(1, 1, 1));

	//生成攻击道具
	if (IsValid(FarAttackPropClass))
	{
		ASandBox_Prop* AttackProp = Cast<ASandBox_Prop>(GetWorld()->SpawnActor(FarAttackPropClass, &SpawnTransform));
		if (AttackProp)
		{
			AttackProp->InitialPropToApply(EnemyTypes,AllyTypes, CollisionDetectionComponent->MoveIgnoreActors);
			AttackProp->MoveToLocation(AttackLocation);
		}

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UnValid FarAttackPropClass"));
	}
}
