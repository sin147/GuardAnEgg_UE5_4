// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxPeople.h"
#include "Components/SkeletalMeshComponent.h"
#include "Props/SandBox_Prop.h"
#include "GameFramework/PlayerState.h"
#include "UI/UI_SandBoxPeopleHP.h"

ASandBoxPeople::ASandBoxPeople()
{
	RootComponent= CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// 设置默认骨骼网格组件
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	//设置默认碰撞组件
	CollisionDetectionComponent= CreateDefaultSubobject<USphereComponent>(TEXT("CollisionDetectionComponent"));
	CollisionDetectionComponent->ComponentTags.Add(FName("Collision"));
	CollisionDetectionComponent->SetupAttachment(RootComponent);
	//设置默认血条控件
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetWidgetClass(UUI_SandBoxPeopleHP::StaticClass());
	HPBarWidget->SetupAttachment(RootComponent);
	HPBarWidget->SetCollisionProfileName(FName("NoCollision"));
	// 设置默认状态
	CurrentState = EPeopleState::Idle;
	// 设置默认属性
	CurrentlyHP = 100.0f;
	MaxHP = 100.0f;
	MoveSpeed = 200.0f;
	RotateSpeed = 180.0f;
	AttackPower = 20.0f;
	// 设置默认动画组合
	AnimMontages.Add(EPeopleState::Idle, nullptr);
	AnimMontages.Add(EPeopleState::Attack, nullptr);
	AnimMontages.Add(EPeopleState::Move, nullptr);
	AnimMontages.Add(EPeopleState::Dead, nullptr);
	AnimMontages.Add(EPeopleState::BeHit, nullptr);
	// 设置攻击范围和索敌范围
	AttackDetectionRadius = 88.0f;
	EnemyDetectionRadius = 500.0f;
	//设置默认阵营
	CurrentCamp = EPeopleCamp::PeopleCamp_None;
	//创建移动时间轴
	MoveTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveTimeLine"));
	MoveCurve = LoadObject<UCurveFloat>(this, TEXT("/Script/Engine.CurveFloat'/Niagara/DefaultAssets/Curves/Templates/LinearRampUp.LinearRampUp'"));
	//创建转向时间轴FF
	RotateTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotateTimeLine"));
	RotateCurve = LoadObject<UCurveFloat>(this, TEXT("/Script/Engine.CurveFloat'/Niagara/DefaultAssets/Curves/Templates/LinearRampUp.LinearRampUp'"));
}

void ASandBoxPeople::BeginPlay()
{
	Super::BeginPlay();
	AllyTypes.Add(CurrentCamp);
	// 初始化血条控件
	if (HPBarWidget)
	{
		Cast<UUI_SandBoxPeopleHP>(HPBarWidget->GetWidget())->SetHP(MaxHP, CurrentlyHP);
	}
	if (CollisionDetectionComponent)
	{
		CollisionDetectionComponent->MoveIgnoreActors.Add(this);
	}
	//绑定移动时间轴相关时间
	if (MoveTimeLine&& MoveCurve)
	{
		//设置时间轴更新函数绑定
		FOnTimelineFloat MoveTimeLineCallBack;
		MoveTimeLineCallBack.BindUFunction(this, FName("OnMoveTimeLineUpdate"));
		UE_LOG(LogTemp, Log, TEXT("绑定移动时间轴更新函数: %s"), *MoveTimeLineCallBack.GetFunctionName().ToString());
		//设置时间轴结束函数绑定
		FOnTimelineEvent MoveTimeLineFinishd;
		MoveTimeLineFinishd.BindUFunction(this, FName("OnMoveTimeLineEnd"));
		UE_LOG(LogTemp, Log, TEXT("绑定移动时间轴结束函数: %s"), *MoveTimeLineFinishd.GetFunctionName().ToString());
		//绑定结束和更新函数到时间轴
		MoveTimeLine->AddInterpFloat(MoveCurve,MoveTimeLineCallBack);
		MoveTimeLine->SetTimelineFinishedFunc(MoveTimeLineFinishd);
		//初始化播放速度
		MoveTimeLine->SetPlayRate(1);
		//初始化循环播放
		MoveTimeLine->SetLooping(false);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("移动曲线为设置"));
	}
	//绑定转向组件
	if (RotateTimeLine && RotateCurve)
	{
		//设置时间轴更新函数绑定
		FOnTimelineFloat RotateTimeLineCallBack;
		RotateTimeLineCallBack.BindUFunction(this, FName("OnRotateTimeLineUpdate"));
		UE_LOG(LogTemp, Log, TEXT("绑定旋转时间轴更新函数: %s"), *RotateTimeLineCallBack.GetFunctionName().ToString());
		//设置时间轴结束函数绑定
		FOnTimelineEvent RotateTimeLineFinishd;
		RotateTimeLineFinishd.BindUFunction(this, FName("OnRoateTimeLineEnd"));
		UE_LOG(LogTemp, Log, TEXT("绑定旋转时间轴结束函数: %s"), *RotateTimeLineFinishd.GetFunctionName().ToString());
		//绑定结束和更新函数到时间轴
		RotateTimeLine->AddInterpFloat(RotateCurve, RotateTimeLineCallBack);
		RotateTimeLine->SetTimelineFinishedFunc(RotateTimeLineFinishd);
		//初始化播放速度
		RotateTimeLine->SetPlayRate(1);
		//初始化循环播放
		RotateTimeLine->SetLooping(false);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("移动曲线为设置"));
	}
}

void ASandBoxPeople::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//敌人检测
	if (EnableEnemyDetection)
	{
		EnableAttackDetection=EnemyDetection();
	}
	//攻击检测
	if (EnableAttackDetection)
	{
		AttackDetection();
	}
	//将HPWidget始终对准相机
	if (HPBarWidget)
	{
		HPBarWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(HPBarWidget->GetComponentLocation(), HPBarWidget->GetComponentLocation() - GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector() * 100));
	}

}

void ASandBoxPeople::OnEnemyDetected(ASandBoxPeople* DetectedPeople)
{
	if (!IsValidPeople(CurrentlyLockedEnemy))
	{
		//处理敌人检测逻辑
		CurrentlyLockedEnemy = DetectedPeople;
	}

}

void ASandBoxPeople::OnAllyDetected(ASandBoxPeople* DetectedPeople)
{
	//CollisionDetectionComponent->MoveIgnoreActors.Add(DetectedPeople);
	UE_LOG(LogTemp, Log, TEXT("%s:检测到友军"),*GetName());
}

void ASandBoxPeople::OnAttackDetected(ASandBoxPeople* DetectedPeople)
{
	if(!IsValidPeople( CurrentlyAttackingEnemy))
	{
		//处理攻击检测逻辑
		CurrentlyAttackingEnemy = DetectedPeople;
	
	}
	CurrentlyLockedEnemy = nullptr;
}


void ASandBoxPeople::OnMoveTimeLineUpdate(float Value)
{
	SetActorLocation(FMath::Lerp(BeginMoveLocation, FVector(EndMoveLocation.X, EndMoveLocation.Y, BeginMoveLocation.Z), Value));
}

void ASandBoxPeople::OnMoveTimeLineEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(MoveUpdateTimer);
}

void ASandBoxPeople::OnRotateTimeLineUpdate(float Value)
{
	SetActorRotation(UKismetMathLibrary::RLerp(BeginRotation, EndRotation, Value, true));
}

void ASandBoxPeople::OnRotateTimeLineEnd()
{
}

bool ASandBoxPeople::EnemyDetection()
{
	EnableEnemyDetection = false;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	//命中结果
	TArray < FHitResult> HitResults;
	//判断攻击范围内部是否还有敌人
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), EnemyDetectionRadius, ObjectTypes, false, CollisionDetectionComponent->GetMoveIgnoreActors(), EDrawDebugTrace::None, HitResults, true);
	for (FHitResult HitResult : HitResults)
	{
		ASandBoxPeople* SandBoxPeople = Cast<ASandBoxPeople>(HitResult.GetActor());
		//判断当前敌人是否为敌人切有效
		if (IsValidPeople(SandBoxPeople))
		{
			for (EPeopleCamp EnemyType : EnemyTypes)
			{
				if (SandBoxPeople->GetCurrentCamp() == EnemyType)
				{
					OnEnemyDetected(SandBoxPeople);
					UE_LOG(LogTemp, Log, TEXT("检测到敌人"));
					return true;
				}
			}
			//判断是否为友军
			for (EPeopleCamp AllyType : AllyTypes)
			{
				if (SandBoxPeople->GetCurrentCamp() == AllyType)
				{
					OnAllyDetected(SandBoxPeople);
					break;
				}
			}
		}
		

	}
	EnableEnemyDetection = true;
	return false;
}

bool ASandBoxPeople::AttackDetection()
{
	//先关闭检测
	EnableAttackDetection = false;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	//命中结果
	TArray < FHitResult> HitResults;
	//判断攻击范围内部是否还有敌人
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation(), AttackDetectionRadius, ObjectTypes, false, CollisionDetectionComponent->GetMoveIgnoreActors(), EDrawDebugTrace::None, HitResults, true);
	for (FHitResult HitResult : HitResults)
	{
		ASandBoxPeople* SandBoxPeople = Cast<ASandBoxPeople>(HitResult.GetActor());
		if (HitResult.GetComponent()->ComponentHasTag("Collision")&&IsValidPeople(SandBoxPeople))
		{
			//判断当前敌人是否为敌人
			for (EPeopleCamp EnemyType : EnemyTypes)
			{
				if (SandBoxPeople->GetCurrentCamp() == EnemyType)
				{
					OnAttackDetected(SandBoxPeople);
					return true;
				}
			}
		}
	}
	//若未检测到就开启检测
	EnableAttackDetection = true;
	return false;
}


void ASandBoxPeople::ApplyRestoreHP(float InRestoreValue)
{
	if (IsValidPeople(this))
	{
		CurrentlyHP += InRestoreValue;
		SkeletalMeshComponent->SetScalarParameterValueOnMaterials("BeGreen", 1);
		Cast<UUI_SandBoxPeopleHP>(HPBarWidget->GetWidget())->SetHP(MaxHP, CurrentlyHP);
		//设置回复逻辑
		TWeakObjectPtr<ASandBoxPeople> WeakThis(this);
		GetWorld()->GetTimerManager().SetTimer(
			BeGreenTimer,
			[WeakThis]() {
				if (ASandBoxPeople* People = WeakThis.Get()) {
					People->SkeletalMeshComponent->SetScalarParameterValueOnMaterials("BeGreen", 0); // 安全调用
				}
			},
			0.4,
			false
		);
		UE_LOG(LogTemp, Log, TEXT("回复"));
	}
}

void ASandBoxPeople::OnBeHit()
{
	SkeletalMeshComponent->SetScalarParameterValueOnMaterials("BeRed", 1);
	//设置闪红逻辑
	TWeakObjectPtr<ASandBoxPeople> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(
		BeRedTimer,
		[WeakThis]() {
			if (ASandBoxPeople* People = WeakThis.Get()) {
				People->SkeletalMeshComponent->SetScalarParameterValueOnMaterials("BeRed", 0); // 安全调用
			}
		},
		0.4,
		false
	);
	UE_LOG(LogTemp, Log, TEXT("闪红"));
}

bool ASandBoxPeople::ManualDetectionAttackEnemy()
{
	return AttackDetection();
}

ASandBoxPeople* ASandBoxPeople::GetCurrentlyAttackingEnemy()
{
	return CurrentlyAttackingEnemy;
}

void ASandBoxPeople::SwitchToAttackState()
{
	if (CurrentState != EPeopleState::Attack&& CurrentState!=EPeopleState::Dead)
	{
		//UE_LOG(LogTemp, Log, TEXT("切换为攻击状态"));
		PreviousState = CurrentState;
		CurrentState = EPeopleState::Attack;
	}
}

void ASandBoxPeople::SwitchToMoveState()
{
	if (CurrentState != EPeopleState::Move && CurrentState != EPeopleState::Dead)
	{
		//UE_LOG(LogTemp, Log, TEXT("切换为移动状态"));
		PreviousState = CurrentState;
		CurrentState = EPeopleState::Move;
	}
} 


void ASandBoxPeople::SwitchToDeadState()
{
	if (CurrentState != EPeopleState::Dead)
	{
		//UE_LOG(LogTemp, Log, TEXT("切换为死亡状态"));
		PreviousState = CurrentState;
		CurrentState = EPeopleState::Dead;
	}
}

void ASandBoxPeople::SwitchToIdleState()
{
	if (CurrentState != EPeopleState::Idle && CurrentState != EPeopleState::Dead)
	{
		//UE_LOG(LogTemp, Log, TEXT("切换为待机状态"));
		if (MoveTimeLine->IsPlaying())
		{
			MoveTimeLine->Stop();
		}
		PreviousState = CurrentState;
		CurrentState = EPeopleState::Idle;
	}
}

void ASandBoxPeople::SwitchToBeHitState()
{
	if (CurrentState != EPeopleState::BeHit && CurrentState != EPeopleState::Dead)
	{
		UE_LOG(LogTemp, Log, TEXT("切换为受击状态"));
		PreviousState = CurrentState;
		CurrentState = EPeopleState::BeHit;
	}
}

void ASandBoxPeople::SwithToPreviousState()
{
	//UE_LOG(LogTemp, Log, TEXT("切换为上一个状态"));
	if (CurrentState != EPeopleState::Dead&& CurrentState!=EPeopleState::Move)
	{
		CurrentState = PreviousState;
	}
}

float ASandBoxPeople::StoreDamage(float InDamage)
{
	return CurrentlyHP-(Damage += InDamage);
}

void ASandBoxPeople::MoveToEnemy()
{
	if (!MoveTimeLine->IsPlaying()&&CurrentState==EPeopleState::Move)
	{
		//初始化开始位置
		BeginMoveLocation = GetActorLocation();
		//初始化结束位置
		//判断当前锁定敌人是否有效
		if (IsValidPeople(CurrentlyLockedEnemy))
		{
			//检测目标非EnemyKing就关闭移动碰撞
			EnableEnemyDetection = false;
			//当前敌人有效向其移动
			EndMoveLocation = CurrentlyLockedEnemy->GetActorLocation();
		}
		else if (IsValidPeople(EnemyKing))
		{
			//如果切换为敌方王了就开启移动碰撞检测
			EnableEnemyDetection = true;
			//当前敌人无效向对方王移动
			EndMoveLocation = EnemyKing->GetActorLocation();
		}
		else
		{
			SwitchToIdleState();
			UE_LOG(LogTemp, Error, TEXT("未设置EnemyKing"));
			return;
		}
		float Distance= FVector::Dist(BeginMoveLocation, EndMoveLocation);
		//根据距离和移动速度设置移动时间
		MoveTimeLine->SetPlayRate(MoveSpeed/Distance );
		//弱引用this防止定时器野指针
		TWeakObjectPtr <ASandBoxPeople> WeakThis=this;

		//检测敌人位置方式变化时跟踪
		GetWorld()->GetTimerManager().SetTimer(
			MoveUpdateTimer,
			[WeakThis]() // 捕获弱引用
			{
				if (WeakThis.IsValid()&& WeakThis->MoveTimeLine && WeakThis->MoveTimeLine->IsPlaying())
				{
					if (IsValidPeople(WeakThis->CurrentlyLockedEnemy) && !(WeakThis->CurrentlyLockedEnemy->GetActorLocation().Equals(WeakThis->EndMoveLocation, 10.f)))
					{
						
						WeakThis->MoveTimeLine->Stop();
						WeakThis->MoveToEnemy();
					}
					else if (IsValidPeople(WeakThis->EnemyKing) && !(WeakThis->EnemyKing->GetActorLocation().Equals(WeakThis->EndMoveLocation, 10.f)))
					{
						WeakThis->MoveTimeLine->Stop();
						WeakThis->MoveToEnemy();
					}
				}
				else
				{
					if (WeakThis->MoveUpdateTimer.IsValid())
					{
						WeakThis->GetWorld()->GetTimerManager().ClearTimer(WeakThis->MoveUpdateTimer);
					}
			
				}
			},
			0.1f, // 检测间隔
			true  // 循环
		);
		MoveTimeLine->PlayFromStart();
		RotateToVector(EndMoveLocation);
	}
}

void ASandBoxPeople::AttackEnemy()
{
	if (IsValidPeople(CurrentlyAttackingEnemy))
	{
		//存储当前受到的伤害
		CurrentlyAttackingEnemy->StoreDamage(AttackPower);
		CurrentlyAttackingEnemy->SwitchToBeHitState();
	}
}

void ASandBoxPeople::RotateToVector(FVector TargetVector)
{
	//初始化起始旋转
	 BeginRotation = GetActorRotation();
	//初始化目标旋转
	EndRotation=UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetVector);
	//用最大旋转角度和旋转速度限制旋转时间
	float MaxAngle= FMath::Max3(FMath::Abs(BeginRotation.Yaw - EndRotation.Yaw), FMath::Abs(BeginRotation.Pitch - EndRotation.Pitch), FMath::Abs(BeginRotation.Roll - EndRotation.Roll));
	MaxAngle = MaxAngle >= 360 ? MaxAngle - 360 : MaxAngle;
	RotateTimeLine->SetPlayRate(RotateSpeed /FMath::Abs(MaxAngle));
	RotateTimeLine->PlayFromStart();
}

void ASandBoxPeople::RotateToEnemy()
{
	if (CurrentlyAttackingEnemy)
	{
		RotateToVector(CurrentlyAttackingEnemy->GetActorLocation());
	}
	else if(EnemyKing)
	{
		RotateToVector(EnemyKing->GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("无目标敌人"));
	}
	
}

void ASandBoxPeople::StopMove()
{
	if (MoveTimeLine->IsPlaying())
	{
		MoveTimeLine->Stop();
	}
}

void ASandBoxPeople::StopAttack()
{
	EnableAttackDetection = true;
	EnableEnemyDetection = true;
	CurrentlyAttackingEnemy = nullptr;
}

void ASandBoxPeople::ApplyDamage()
{
	CurrentlyHP -= (Damage - DefensePower>0? Damage - DefensePower:0);
	Cast<UUI_SandBoxPeopleHP>(HPBarWidget->GetWidget())->SetHP(MaxHP, CurrentlyHP);
	//当血量少于0死亡
	if (CurrentlyHP<=0)
	{
		SwitchToDeadState();
	}
	Damage = 0;
}


void ASandBoxPeople::SetEnemyKing(ASandBoxPeople* InEnemyKing)
{
	EnemyKing = InEnemyKing;
}

bool ASandBoxPeople::IsFriend(ASandBoxPeople* InPeople)
{
	for (EPeopleCamp AllyType:AllyTypes )
	{
		if (InPeople->GetCurrentCamp() == AllyType)
		{
			return true;
		}
	}

	return false;
}

bool ASandBoxPeople::IsEnemy(ASandBoxPeople* InPeople)
{
	for (EPeopleCamp EnemyType : EnemyTypes)
	{
		if (InPeople->GetCurrentCamp() == EnemyType)
		{
			return true;
		}
	}

	return false;
}


