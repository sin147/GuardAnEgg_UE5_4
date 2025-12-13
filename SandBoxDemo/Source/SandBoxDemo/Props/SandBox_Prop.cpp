// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/SandBox_Prop.h"
ASandBox_Prop::ASandBox_Prop()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(FName("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	MoveTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveTimeLine"));
	MoveCurve = LoadObject<UCurveFloat>(this, TEXT("/Script/Engine.CurveFloat'/Niagara/DefaultAssets/Curves/Templates/LinearRampUp.LinearRampUp'"));
}

void ASandBox_Prop::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(CollisionComponent))
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASandBox_Prop::OnOverlap);
		CollisionComponent->MoveIgnoreActors.Add(this);
	}
	//初始化移动时间轴
	if (MoveTimeLine && MoveCurve)
	{
		//初始化更新时间
		FOnTimelineFloat MoveTimeLineFloat;
		MoveTimeLineFloat.BindUFunction(this, "OnMoveUpdate");
		MoveTimeLine->AddInterpFloat(MoveCurve, MoveTimeLineFloat);
		//初始化结束时间
		FOnTimelineEvent  OnMovetTimeLineEnd;
		OnMovetTimeLineEnd.BindUFunction(this, "OnMoveEnd");
		MoveTimeLine->SetTimelineFinishedFunc(OnMovetTimeLineEnd);

	}
	else
	{
		if (!MoveTimeLine)
		{
			UE_LOG(LogTemp, Error, TEXT("MoveTimeLine Is Not Vaild"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MoveCurve Is Not Vaild"));
		}

	}

}

void ASandBox_Prop::OnMoveUpdate(float value)
{
	SetActorLocation(UKismetMathLibrary::VLerp(BeginLocation, EndLocation, value));
}

void ASandBox_Prop::OnMoveEnd()
{
	
	if (ShotPoint)
	{
		//触发爆点
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShotPoint, EndLocation);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
		TArray<TObjectPtr<ASandBoxPeople>>Enemys;
		TArray<TObjectPtr<ASandBoxPeople>>Allys;
		//命中结果
		TArray < FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), CollisionComponent->GetScaledSphereRadius()* ApplyScale, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, HitResults, true);
		for (FHitResult ApplyHitResult : HitResults)
		{
			ASandBoxPeople* SandBoxPeople = Cast<ASandBoxPeople>(ApplyHitResult.GetActor());
			ASandBoxCharacter* SandBoxCharacter = Cast<ASandBoxCharacter>(ApplyHitResult.GetActor());
			if (SandBoxPeople)
			{
				if (EnemyTypes.IsEmpty())
				{
					UE_LOG(LogTemp, Error, TEXT("未设置敌人类型"));

				}
				if (AllyTypes.IsEmpty())
				{
					UE_LOG(LogTemp, Error, TEXT("未设置友军类型"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("友军类数%d"), AllyTypes.Num());
				}
				//检测敌人
				for (EPeopleCamp EnemyType : EnemyTypes)
				{
					if (SandBoxPeople->GetCurrentCamp() == EnemyType)
					{
						Enemys.Add(SandBoxPeople);
					}
				}
				//检测友军
				for (EPeopleCamp AllyType : AllyTypes)
				{
					if (SandBoxPeople->GetCurrentCamp() == AllyType)
					{
						Allys.Add(SandBoxPeople);
					}

				}
				UE_LOG(LogTemp, Log, TEXT("道具影响友军人数%d，道具影响敌军人数%d"), Allys.Num(), Enemys.Num());
			}
			else if (SandBoxCharacter)
			{
				for (EPeopleCamp EnemyCamp : EnemyTypes)
				{
					if (SandBoxCharacter->CurrentCamp == EnemyCamp)
					{
						SandBoxCharacter->BeHit(DamageValue);
					}
				}
			}
		}
		//UE_LOG(LogTemp, Log, TEXT("道具影响人数%d"), SandBoxPeoples.Num());
		ApplyToPeoples(Enemys, Allys);
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("未设置爆点"));
		Destroy();
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("道具移动结束"));
}

void ASandBox_Prop::ApplyToPeoples(TArray<TObjectPtr<ASandBoxPeople>> Enemys, TArray<TObjectPtr<ASandBoxPeople>> Allys)
{
	if (DamageValue > 0)
	{
		for (ASandBoxPeople* Enemy : Enemys)
		{
			//应用攻击

			Enemy->StoreDamage(DamageValue);
			Enemy->SwitchToBeHitState();

		}
	}
	if (RestoreValue > 0)
	{
		for (ASandBoxPeople* Ally : Allys)
		{
			//应用回复
			Ally->ApplyRestoreHP(RestoreValue);
		}
	}

}

void ASandBox_Prop::InitialPropToApply(TArray<EPeopleCamp> InEnemyTypes, TArray<EPeopleCamp> InAllyTypes, TArray<TObjectPtr< AActor>> InIgnoreActors)
{
	AllyTypes = InAllyTypes;
	EnemyTypes = InEnemyTypes;
	CollisionComponent->MoveIgnoreActors = InIgnoreActors;
}

void ASandBox_Prop::OnOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{

	ASandBoxCharacter* SandBoxCharacter = Cast<ASandBoxCharacter>(HitResult.GetActor());
	ASandBoxPeople* SandBoxPeople = Cast<ASandBoxPeople>(HitResult.GetActor());
	//检测是否为AIPeople
	if (SandBoxPeople)
	{
		
		for (EPeopleCamp EnemyCamp : EnemyTypes)
		{
			if (SandBoxPeople->GetCurrentCamp() == EnemyCamp)
			{
				CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				MoveTimeLine->Stop();
				MoveToLocation(SandBoxPeople->GetActorLocation());
				UE_LOG(LogTemp, Log, TEXT("碰到敌人"));
			}
		}

	}
	else if (SandBoxCharacter)
	{
		for (EPeopleCamp EnemyCamp : EnemyTypes)
		{
			if (SandBoxCharacter->CurrentCamp== EnemyCamp)
			{
				CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				MoveTimeLine->Stop();
				MoveToLocation(SandBoxCharacter->GetActorLocation());
			}
		}
	}
	return;
}

void ASandBox_Prop::MoveToLocation(FVector Location)
{
	//初始化开始地点
	BeginLocation = GetActorLocation();
	//初始化结束地点
	EndLocation = Location;
	//计算两点距离
	float Distance = FVector::Distance(BeginLocation, EndLocation);
	//根据距离和速度计算移动时间
	MoveTimeLine->SetPlayRate(MoveSpeed / Distance);
	if (!MoveTimeLine->IsPlaying())
	{
		MoveTimeLine->PlayFromStart();
	}
}
