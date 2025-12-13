// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Props/SandBox_Prop.h"
#include "UI/UI_CharacterInfo.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ASandBoxCharacter::ASandBoxCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CharacterRotationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotationTimeline"));
	RotationCurve= LoadObject<UCurveFloat>(this, TEXT("/Script/Engine.CurveFloat'/Niagara/DefaultAssets/Curves/Templates/LinearRampUp.LinearRampUp'"));
}

void ASandBoxCharacter::SwitchCloseAttack()
{
	if (CurrentlyAttackType == EAttackType::AT_None)
	{
		CurrentlyAttackType = EAttackType::AT_CloseAttack;
	}
}

void ASandBoxCharacter::CloseAttack()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	//命中结果
	//命中结果
	TArray < FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetActorLocation(), GetActorLocation()+GetActorForwardVector()*200, 100, ObjectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, HitResults, true);
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
			//检测敌人
			for (EPeopleCamp EnemyType : EnemyTypes)
			{
				if (SandBoxPeople->GetCurrentCamp() == EnemyType)
				{
					SandBoxPeople->StoreDamage(20);
					SandBoxPeople->SwitchToBeHitState();
				}
			}
		}
		else if (SandBoxCharacter)
		{
			for (EPeopleCamp EnemyCamp : EnemyTypes)
			{
				if (SandBoxCharacter->CurrentCamp == EnemyCamp)
				{
					SandBoxCharacter->BeHit(20);
				}
			}
		}
	}
}

void ASandBoxCharacter::SwitchFarAttack()
{
	if (CurrentlyAttackType == EAttackType::AT_None)
	{
		CurrentlyAttackType = EAttackType::AT_FarAttack;
	}
	
}

void ASandBoxCharacter::FarAttack()
{
	//初始化攻击目标位置
	FVector AttackLocation = GetActorForwardVector() * 1500 + GetActorLocation();
	//道具生成Transform
	FTransform SpawnTransform(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackLocation), GetActorLocation() + FVector(0, 0, 30), FVector(1, 1, 1));

	//生成攻击道具
	if (IsValid(FarAttackPropClass))
	{
		ASandBox_Prop* AttackProp = Cast<ASandBox_Prop>(GetWorld()->SpawnActor(FarAttackPropClass, &SpawnTransform));
		if (AttackProp)
		{
			TArray<TObjectPtr<AActor>> IgnoreActors;
			IgnoreActors.Add(this);
			AttackProp->InitialPropToApply(EnemyTypes, AllyTypes, IgnoreActors);
			AttackProp->MoveToLocation(AttackLocation);
		}

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UnValid FarAttackPropClass"));
	}
}

void ASandBoxCharacter::Move(const FInputActionValue& Value)
{
	FVector InputValue = Value.Get<FVector>();
	//保持相机不变
	//FRotator TempRotator=SpringArmComponent->GetComponentRotation();

	AddMovementInput(GetActorForwardVector(), InputValue.X );
	UE_LOG(LogTemp, Log, TEXT("Move: %s"), *InputValue.ToString());
}

void ASandBoxCharacter::Look(const FInputActionValue& Value)
{
	
	FVector InputValue = Value.Get<FVector>();
	UE_LOG(LogTemp, Log, TEXT("锁定角色旋转:X:%lf,Y:%lf,Z:%lf"), InputValue.X, InputValue.Y, InputValue.Z);
	if (InputValue.Z>0)
	{
		
		BlockLookRotation = SpringArmComponent->GetComponentRotation();
		CharacterRotationTimeline->Stop();
		SpringArmComponent->SetWorldRotation(SpringArmComponent->GetComponentRotation() + FRotator(InputValue.Y, InputValue.X, 0));
		//UE_LOG(LogTemp, Log, TEXT("锁定角色旋转:Y:%lf"),InputValue.Y);
	}
	else if (InputValue.Z <0)
	{
		SpringArmComponent->SetWorldRotation(BlockLookRotation);
		//UE_LOG(LogTemp, Log, TEXT("松开复位:Y:%lf"), InputValue.Y);
	}
	else
	{
		SpringArmComponent->SetWorldRotation(SpringArmComponent->GetComponentRotation() + FRotator(InputValue.Y, InputValue.X, 0));
		UpdateCharacterRotation();
	
	}
	//UE_LOG(LogTemp, Log, TEXT("Look: %s"),*SpringArmComponent->GetComponentRotation().ToString());
}

void ASandBoxCharacter::UpdateRotation(float value)
{
	if (!GetVelocity().IsZero())
	{
		GetController()->ClientSetRotation(UKismetMathLibrary::RLerp(BeginRotation, TargetRotation+FRotator(10, 0, 0), value, true));
	}

	//SetActorRotation(UKismetMathLibrary::RLerp(BeginRotation, TargetRotation, value,true));
	SpringArmComponent->SetWorldRotation(TargetRotation);
	//UE_LOG(LogTemp, Log, TEXT("%s"), *TargetRotation.ToString());
}

void ASandBoxCharacter::UpdateRotationFinish()
{
	//SetActorRotation(TargetRotation);
	UE_LOG(LogTemp, Log, TEXT("%s_%s"), *GetActorRotation().ToString(), *TargetRotation.ToString());
}

void ASandBoxCharacter::UpdateCharacterRotation()
{
	if (CharacterRotationTimeline->IsPlaying())
	{
		CharacterRotationTimeline->Stop();
	}

	//初始化转向
	BeginRotation = GetActorRotation();
	TargetRotation = SpringArmComponent->GetComponentRotation();
	CharacterRotationTimeline->PlayFromStart();
}


// Called when the game starts or when spawned
void ASandBoxCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentlyHP = MaxHP;
	//绑定转向
	if (CharacterRotationTimeline&& RotationCurve)
	{
		FOnTimelineFloat UpdateRotation;
		UpdateRotation.BindUFunction(this, FName("UpdateRotation"));
		FOnTimelineEvent UpdateRotationFinish;
		UpdateRotationFinish.BindUFunction(this, FName("UpdateRotationFinish"));
		CharacterRotationTimeline->AddInterpFloat(RotationCurve, UpdateRotation);
		CharacterRotationTimeline->SetTimelineFinishedFunc(UpdateRotationFinish);
		UE_LOG(LogTemp, Log, TEXT("绑定转向时间轴"));
	}
	//初始化UI
	if (CharacterInfoClass)
	{
		UUI_CharacterInfo*CharacterInfo=CreateWidget<UUI_CharacterInfo>(GetWorld(), CharacterInfoClass);
		CharacterInfo->AddToViewport();
		CharacterInfo->Initialize(this);
		UE_LOG(LogTemp, Error, TEXT("生成CharacterInfoWidget"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("为设置CharacterInfoClass"));
	}
}

void ASandBoxCharacter::BeHit(float Damage)
{
	CurrentlyHP -= Damage;
	GetMesh()->SetScalarParameterValueOnMaterials("BeRed", 1);
	//设置闪红逻辑
	TWeakObjectPtr<ASandBoxCharacter> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(
		BeRedTimer,
		[WeakThis]() {
			if (ASandBoxCharacter* People = WeakThis.Get()) {
				People->GetMesh()->SetScalarParameterValueOnMaterials("BeRed", 0); // 安全调用
			}
		},
		0.4,
		false
	);
}

// Called every frame
void ASandBoxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//更新转向
	

}

// Called to bind functionality to input
void ASandBoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASandBoxCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASandBoxCharacter::Look);
		EnhancedInputComponent->BindAction(CloseAttackAction, ETriggerEvent::Triggered, this, &ASandBoxCharacter::SwitchCloseAttack);
		EnhancedInputComponent->BindAction(FarAttackAction, ETriggerEvent::Triggered, this, &ASandBoxCharacter::SwitchFarAttack);
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				if (InputMappingContext)
				{
					Subsystem->AddMappingContext(InputMappingContext, 0);
					UE_LOG(LogTemp, Log, TEXT("应用开始"));
				}
			}
		}
	}

}

