// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Props/SandBox_Prop.h"
#include "UI/UI_CharacterInfo.h"
#include "Components/ArrowComponent.h"
#include "GameplayTagContainer.h"
#include "SandBoxPlayerController.h"
#include "System/InteractiveSubsystem.h"
#include "System/CharacterSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASandBoxCharacter::ASandBoxCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//*********************************组件
	//状态机
	StateMachine = CreateDefaultSubobject<UStateMachineBase>(TEXT("CharacterStateMachine"));
	//相机臂
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh(),FName("CG"));
	//相机组件
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	//角色控制点
	CharacterControlPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterControlPoint"));
	CharacterControlPoint->SetupAttachment(RootComponent);
	//技能组件
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	GetMesh()->SetupAttachment(CharacterControlPoint);

}

bool ASandBoxCharacter::ActivateAbilityByTag(FGameplayTag AbilityTag)
{
	if (CharacterDataAsset->IsVaildAbility(AbilityTag) && AbilitySystemComponent->TryActivateAbilityByClass(CharacterDataAsset->Abilities[AbilityTag], false))
	{
		UE_LOG(LogTemp, Log, TEXT("Active %s"), *AbilityTag.GetTagName().ToString());
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Not Give %s"), *AbilityTag.GetTagName().ToString());
		return false;
	}
	
}

void ASandBoxCharacter::InitalAbility()
{
	if (!IsValid(CharacterDataAsset)){return;}

	TMap<FGameplayTag, TSubclassOf<UGameplayAbility>>  Abilities = CharacterDataAsset->Abilities;
	for (TPair<FGameplayTag, TSubclassOf<UGameplayAbility>> Ability : Abilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.Value));
	}
}

void ASandBoxCharacter::InitAttribute()
{
	//数据资产
	CharacterDataAsset = UCharacterDataAsset::DeepCopy(CharacterDataAsset);
	UCharacterMovementComponent* CharacterMovementComponent =GetCharacterMovement();
	ASandBoxPlayerController* SandBoxPlayerController = GetController< ASandBoxPlayerController>();
	if (CharacterDataAsset and CharacterMovementComponent)
	{
		//工具数据资产初始化属性
		//飞行速度
		CharacterMovementComponent->MaxFlySpeed = CharacterDataAsset->GetAttributeByEnum(EAttribute::FlySpeed).GetValueByValueType(ECAVType::CAVT_Max)* CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed;
		//步行速度
		CharacterMovementComponent->MaxWalkSpeed = CharacterDataAsset->GetAttributeByEnum(EAttribute::WalkSpeed).GetValueByValueType(ECAVType::CAVT_Max) * CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed;
		//游泳速度
		CharacterMovementComponent->MaxSwimSpeed = CharacterDataAsset->GetAttributeByEnum(EAttribute::SwimmingSpeed).GetValueByValueType(ECAVType::CAVT_Max) * CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed;
	}
	if (CharacterDataAsset and SandBoxPlayerController)
	{
		//左右旋转速度
		SandBoxPlayerController->MaxRotationRate.Yaw = CharacterDataAsset->GetAttributeByEnum(EAttribute::YawRotatorSpeed).GetValueByValueType(ECAVType::CAVT_Max);
		//上下旋转速度
		SandBoxPlayerController->MaxRotationRate.Pitch = CharacterDataAsset->GetAttributeByEnum(EAttribute::PitchRotatorSpeed).GetValueByValueType(ECAVType::CAVT_Max);
	}
	SetMoveMode(EMovementMode::MOVE_Walking);
}

void ASandBoxCharacter::SetAttributeByEnum(EAttribute InAttribute, float InNewValue, ECAVType InValueType)
{
	if (!IsValid(CharacterDataAsset) or !CharacterDataAsset->IsVaildAttribute(InAttribute))
	{
		return;
	}
	CharacterDataAsset->GetAttributeByEnum(InAttribute).SetValueByValueType(InNewValue, InValueType);
}

float ASandBoxCharacter::GetAttributeByEnum(EAttribute InAttribute, ECAVType InValueType)
{

	if (!IsValid(CharacterDataAsset) or !CharacterDataAsset->IsVaildAttribute(InAttribute)) { return 0; }
	return CharacterDataAsset->GetAttributeByEnum(InAttribute).GetValueByValueType(InValueType);
}

void ASandBoxCharacter::SetCurrentlyHP(float NewHP)
{
	GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->SetAttributeByEnum(this, EAttribute::HP, NewHP, ECAVType::CAVT_Currently);
}

float ASandBoxCharacter::GetCurrentlyHP()
{
	if (GetGameInstance())
	{
		return GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->GetAttributeByEnum(this, EAttribute::HP, ECAVType::CAVT_Currently);
	}
	return 0;
}

void ASandBoxCharacter::UpdateCharacterState(float DeltaTime)
{
	//根据当前移MovementMode判断状态
	float AbsMoveSpeed = FMath::Abs(GetCurrentlyMoveSpeed());
	if (1 < AbsMoveSpeed && AbsMoveSpeed <= GetMaxNormalMoveSpeed())
	{
		StateMachine->EnterState(EState::S_Move);

	}
	else if(AbsMoveSpeed > GetMaxNormalMoveSpeed())
	{
		if (GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->GetAttributeByEnum(this,EAttribute::EnduranceValue, ECAVType::CAVT_Currently) <= 0)
		{
			StopQuick();
		}
		else
		{
			StateMachine->EnterState(EState::S_QuickMove);
		}
	}
	else
	{
		StateMachine->EnterState(EState::S_Idle);
	}

	//UE_LOG(LogTemp, Log, TEXT("MoveSpeed:%lf"), GetSpeed());
}

TEnumAsByte<EState> ASandBoxCharacter::GetCurrentlyChracterState()
{

	return StateMachine->GetCurrentlyState();
}

void ASandBoxCharacter::UpdateAttributes(float DeltaTime)
{
	ASandBoxPlayerController* PlayController = GetController<ASandBoxPlayerController>();
	if (PlayController)
	{
		//更新左右旋转速度
		SetCurrentlyYawRotatorSpeed(PlayController->GetRotationRate().Yaw);
		//UE_LOG(LogTemp, Log, TEXT("%lf"), PlayController->GetRotationRate().Yaw);
		//更新上下速度旋转
		SetCurrentlyPitchRotatorSpeed(PlayController->GetRotationRate().Pitch);
	}
	float EnduranceConsumption = CharacterDataAsset->QuickMoveEnduranceConsumptionValue;
	float MaxEnduranceValue = GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->GetAttributeByEnum(this, EAttribute::EnduranceValue, ECAVType::CAVT_Max);
	float CurrentlyEnduranceValue = GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->GetAttributeByEnum(this, EAttribute::EnduranceValue);
	//快速移动的体力消耗
	if (StateMachine->GetCurrentlyState() != EState::S_QuickMove)
	{
		GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->SetAttributeByEnum(this, EAttribute::EnduranceValue, CurrentlyEnduranceValue + (EnduranceConsumption * MaxEnduranceValue * DeltaTime));
	}
	else
	{
		GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->SetAttributeByEnum(this, EAttribute::EnduranceValue, CurrentlyEnduranceValue - (EnduranceConsumption * MaxEnduranceValue * DeltaTime));
	}
}

void ASandBoxCharacter::SetCurrentlyYawRotatorSpeed(float InRotatorSpeed)
{
	GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->SetAttributeByEnum(this, EAttribute::YawRotatorSpeed, InRotatorSpeed, ECAVType::CAVT_Currently);
}
float ASandBoxCharacter::GetCurrentlyYawRotatorSpeed()
{
	if (GetGameInstance())
	{
		return GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->GetAttributeByEnum(this, EAttribute::YawRotatorSpeed);
	}
	return 0;
}
void ASandBoxCharacter::SetMaxYawRotatorSpeed(float InRotatorSpeed)
{
	GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->SetAttributeByEnum(this, EAttribute::YawRotatorSpeed, InRotatorSpeed, ECAVType::CAVT_Max);
}
float ASandBoxCharacter::GetMaxYawRotatorSpeed()
{
	if (GetGameInstance())
	{
		return GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->GetAttributeByEnum(this, EAttribute::YawRotatorSpeed, ECAVType::CAVT_Max);
	}
	return 0;
}

void ASandBoxCharacter::SetCurrentlyPitchRotatorSpeed(float InRotatorSpeed)
{
	GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->SetAttributeByEnum(this, EAttribute::PitchRotatorSpeed, InRotatorSpeed);
}
float ASandBoxCharacter::GetCurrentlyPitchRotatorSpeed()
{
	if (GetGameInstance())
	{
		return GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->GetAttributeByEnum(this, EAttribute::PitchRotatorSpeed);
	}
	return 0;
}

void ASandBoxCharacter::SetMaxPitchRotatorSpeed(float InRotatorSpeed)
{

	GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->SetAttributeByEnum(this, EAttribute::PitchRotatorSpeed, InRotatorSpeed, ECAVType::CAVT_Max);
}

float ASandBoxCharacter::GetMaxPitchRotatorSpeed()
{
	if (GetGameInstance())
	{
		return GetGameInstance()->GetSubsystem<UAttributeSubsystem>()->GetAttributeByEnum(this, EAttribute::PitchRotatorSpeed, ECAVType::CAVT_Max);
	}
	return 0;
}

float ASandBoxCharacter::GetCurrentlyMovementMaxQuickMoveSpeed()
{

	return GetMaxQuickMoveSpeed();
}

float ASandBoxCharacter::GetCurrentlyMovementMaxNormalMoveSpeed()
{
	return GetMaxNormalMoveSpeed();
}

float ASandBoxCharacter::GetMaxQuickMoveSpeed(EMovementMode InMovementMode)
{
	if (InMovementMode == MOVE_None)
	{
		InMovementMode = GetCharacterMovement()->MovementMode;
	}
	if (GetGameInstance()&&GetGameInstance()->GetSubsystem<UCharacterSubsystem>())
	{
		return GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->GetMoveMaxSpeed(this, InMovementMode) * (IsQuickMove ? 1 : (1 / CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed));
	}
	return 0;
}

float ASandBoxCharacter::GetMaxNormalMoveSpeed(EMovementMode InMovementMode)
{
	if (InMovementMode == MOVE_None)
	{
		InMovementMode = GetCharacterMovement()->MovementMode;
	}
	if (GetGameInstance()&&GetGameInstance()->GetSubsystem<UCharacterSubsystem>())
	{
		return GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->GetMoveMaxSpeed(this, InMovementMode) * (IsQuickMove ? CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed : 1);
	}
	return 0;
}

float ASandBoxCharacter::GetCurrentlyMoveSpeed()
{
		return GetVelocity().Length();
}

void ASandBoxCharacter::SetMoveMode(EMovementMode InNewMode)
{
	if (GetCharacterMovement()->MovementMode != InNewMode)
	{
		GetCharacterMovement()->SetMovementMode(InNewMode);
	}

}

EMovementMode ASandBoxCharacter::GetMoveMode()
{
	return GetCharacterMovement()->MovementMode;
}

void ASandBoxCharacter::SetMoveMaxSpeed(float InSpeed,EMovementMode InMode)
{
	if (InMode == S_None)
	{
		InMode = GetCharacterMovement()->MovementMode;
	}
	IsQuickMove = InSpeed > GetMaxNormalMoveSpeed();
	switch (InMode)
	{
	case MOVE_None:
		break;
	case MOVE_Walking:
		GetCharacterMovement()->MaxWalkSpeed = InSpeed;
		break;
	case MOVE_NavWalking:
		break;

	case MOVE_Swimming:
		GetCharacterMovement()->MaxSwimSpeed = InSpeed;
		break;
	case MOVE_Falling:
	case MOVE_Flying:
		GetCharacterMovement()->MaxFlySpeed = InSpeed;
		break;
	case MOVE_Custom:
		break;
	case MOVE_MAX:
		break;
	default:
		break;
	}
}

float ASandBoxCharacter::GetMoveMaxSpeed(EMovementMode InMode)
{
	if (InMode == S_None)
	{
		InMode = GetCharacterMovement()->MovementMode;
	}
	switch (InMode)
	{
	case MOVE_None:
		break;
	case MOVE_Walking:
		return	GetCharacterMovement()->MaxWalkSpeed;
	case MOVE_NavWalking:
		break;

	case MOVE_Swimming:
		return 	GetCharacterMovement()->MaxSwimSpeed;
	case MOVE_Falling:
	case MOVE_Flying:
		return	GetCharacterMovement()->MaxFlySpeed;
	case MOVE_Custom:
		break;
	case MOVE_MAX:
		break;
	default:
		break;
	}
	return 0;
}

EMovementMode ASandBoxCharacter::GetCurrentlyMoveMode()
{
	if (GetGameInstance())
	{
		return GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->GetMovementMode(this);
	}
	return EMovementMode::MOVE_None;
}

void ASandBoxCharacter::CloseAttack()
{
	//UE_LOG(LogTemp, Log, TEXT("%s : Close Attack"),*GetActorNameOrLabel());
}

void ASandBoxCharacter::FarAttack()
{
	//UE_LOG(LogTemp, Log, TEXT("%s : Far Attack"), *GetActorNameOrLabel());
}

void ASandBoxCharacter::Move(const FInputActionValue& Value)
{
	switch (GetCharacterMovement()->MovementMode)
	{
	case MOVE_None:
		break;
	case MOVE_Walking:
		WalkMove(Value);
		break;
	case MOVE_NavWalking:
		break;
	case MOVE_Falling:
		break;
	case MOVE_Swimming:
	case MOVE_Flying:
		FlyMove(Value);
		break;
	case MOVE_Custom:
		break;
	case MOVE_MAX:
		break;
	default:
		break;
	}

}

void ASandBoxCharacter::WalkMove(const FInputActionValue& InputValue)
{
	FVector Value = InputValue.Get<FVector>();
	if (!IsValid(Controller)) { return; }
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	const FRotator ControllerRotation = Controller->GetControlRotation();
	const FRotator CharacterRotation = GetCharacterRotation();
	// 应用前进和后退输入
	if (Value.X != 0)
	{
		//设置上下偏转限制
		const FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, Value.X);
	}

	//左右旋转
	if (Value.Y != 0&&UKismetMathLibrary::NearlyEqual_FloatFloat(GetVelocity().Z,0,1))
	{
		if (Value.X == 0)
		{
			AddControllerYawInput(Value.Y);
			const FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(ForwardDirection, 0.5);

		}
		else
		{
			AddControllerYawInput(Value.X * Value.Y);
		}
	}
}

void ASandBoxCharacter::FlyMove(const FInputActionValue& InputValue)
{
	FVector Value = InputValue.Get<FVector>();
	if (!IsValid(Controller)) { return; }
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	const FRotator ControllerRotation = Controller->GetControlRotation();
	const FRotator CharacterRotation = GetCharacterRotation();
	// 应用前进和后退输入
	if (Value.X != 0)
	{

		//设置上下偏转限制
		const FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, Value.X);
	}

	//左右旋转
	if (Value.Y != 0)
	{

		if (Value.X == 0)
		{
			AddControllerYawInput(Value.Y);
			const FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(ForwardDirection, 0.0001);

		}
		else
		{
			AddControllerYawInput(Value.X * Value.Y);
		}
	}

	//上下旋转
	if (Value.Z != 0)
	{

		AddControllerPitchInput(Value.Z);
		if (Value.X == 0)
		{
			const FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(ForwardDirection, 0.5);
		}
	}
}

void ASandBoxCharacter::StopMove(const FInputActionValue& InputValue)
{
	GetController()->SetControlRotation(GetCharacterRotation());
}

void ASandBoxCharacter::StartJump()
{
	ActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));
}

void ASandBoxCharacter::StartQuick()
{
	GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this, GetCharacterMovement()->MovementMode, GetMaxQuickMoveSpeed());
}

void ASandBoxCharacter::StopQuick()
{
	//GetCharacterMovement()->MaxAcceleration = 2048;
	GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this,GetCharacterMovement()->MovementMode, GetMaxNormalMoveSpeed());
}

void ASandBoxCharacter::Interact()
{
	GetGameInstance()->GetSubsystem<UInteractiveSubsystem>()->RequestInteract(this);
}

void ASandBoxCharacter::StartSwim()
{
	if (GetCurrentlyMoveMode() != EMovementMode::MOVE_Swimming)
	{
		if (IsQuickMove)
		{
			GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this, EMovementMode::MOVE_Swimming, GetMaxQuickMoveSpeed(EMovementMode::MOVE_Swimming));
		}
		else
		{
			GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this, EMovementMode::MOVE_Swimming, GetMaxNormalMoveSpeed(EMovementMode::MOVE_Swimming));
		}

		GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->ChangeMovementMode(this, EMovementMode::MOVE_Swimming);
	}

}

void ASandBoxCharacter::StopSwim()
{
	if (GetCurrentlyMoveMode() == EMovementMode::MOVE_Swimming)
	{
		if (IsQuickMove)
		{
			GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this, EMovementMode::MOVE_Walking, GetMaxQuickMoveSpeed(EMovementMode::MOVE_Walking));
		}
		else
		{
			GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this, EMovementMode::MOVE_Walking, GetMaxNormalMoveSpeed(EMovementMode::MOVE_Walking));
		}
		GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->ChangeMovementMode(this, EMovementMode::MOVE_Walking);
	}

}

void ASandBoxCharacter::TakeOff(const FInputActionValue& InputValue)
{
	if (GetCurrentlyMoveMode() != EMovementMode::MOVE_Flying)
	{
		if (IsQuickMove)
		{
			GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this, EMovementMode::MOVE_Flying, GetMaxQuickMoveSpeed(EMovementMode::MOVE_Flying));
		}
		else
		{
			GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this, EMovementMode::MOVE_Flying, GetMaxNormalMoveSpeed(EMovementMode::MOVE_Flying));
		}
		//起飞逻辑
		ActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.TakeOff")));
	}
}

void ASandBoxCharacter::Land(const FInputActionValue& InputValue)
{
	if (GetCurrentlyMoveMode() == EMovementMode::MOVE_Flying)
	{
		if (IsQuickMove)
		{
			GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this, EMovementMode::MOVE_Walking, GetMaxQuickMoveSpeed(EMovementMode::MOVE_Walking));
		}
		else
		{
			GetGameInstance()->GetSubsystem<UCharacterSubsystem>()->SetMoveMaxSpeed(this, EMovementMode::MOVE_Walking, GetMaxNormalMoveSpeed(EMovementMode::MOVE_Walking));
		}

		ActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Land")));
	}
}

void ASandBoxCharacter::Look(const FInputActionValue& Value)
{
	//旋转相机臂
	FVector2D InputValue = Value.Get<FVector2D>();
	FRotator NewRotator = SpringArmComponent->GetComponentRotation();
	NewRotator.Pitch = UKismetMathLibrary::ClampAngle(NewRotator.Pitch - InputValue.Y, -80.f, 80.f);
	NewRotator.Yaw += InputValue.X;
	SpringArmComponent->SetWorldRotation(NewRotator);

	// add yaw and pitch input to controller

	//UE_LOG(LogTemp, Log, TEXT("Look: %s"),*SpringArmComponent->GetComponentRotation().ToString());
}

FVector ASandBoxCharacter::GetCharacterRightVector()
{
	return CharacterControlPoint->GetRightVector();
}

FVector ASandBoxCharacter::GetCharacterForwardVector()
{
	return CharacterControlPoint->GetForwardVector();
}

FVector ASandBoxCharacter::GetCharacterUpVector()
{
	return CharacterControlPoint->GetUpVector();
}

FRotator ASandBoxCharacter::GetCharacterRotation()
{
	return CharacterControlPoint->GetComponentRotation();
}

// Called when the game starts or when spawned
void ASandBoxCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAttribute();
	InitalAbility();
}

void ASandBoxCharacter::BeHit(float Damage)
{
	/*float CurrentHP = GetHP();
	CurrentHP -= Damage;
	SetHP(CurrentHP);
	UE_LOG(LogTemp, Log, TEXT("%s Be Hit, Current HP : %f"), *GetActorNameOrLabel(), GetHP());*/
}

// Called every frame
void ASandBoxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled())
	{
		//更新速度
		UpdateAttributes(DeltaTime);
		//更新控制器Rotation
		if (GetVelocity().Equals(FVector::ZeroVector, 10)/*&&!GetCharacterRotation().Equals(GetControlRotation(),10)*/)
		{
			AddMovementInput(GetCharacterForwardVector(), 0.0001);
		}
	}
	//更新角色状态
	UpdateCharacterState(DeltaTime);
}

// Called to bind functionality to input
void ASandBoxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//绑定输入动作
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASandBoxCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASandBoxCharacter::StopMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASandBoxCharacter::Look);
		EnhancedInputComponent->BindAction(CloseAttackAction, ETriggerEvent::Triggered, this, &ASandBoxCharacter::CloseAttack);
		EnhancedInputComponent->BindAction(FarAttackAction, ETriggerEvent::Triggered, this, &ASandBoxCharacter::FarAttack);
		EnhancedInputComponent->BindAction(TakeOffAction, ETriggerEvent::Completed, this, &ASandBoxCharacter::TakeOff);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASandBoxCharacter::StartJump);
		EnhancedInputComponent->BindAction(LandAction, ETriggerEvent::Completed, this, &ASandBoxCharacter::Land);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ASandBoxCharacter::StartQuick);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASandBoxCharacter::StopQuick);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ASandBoxCharacter::Interact);
	}
	//添加输入映射上下文
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

void ASandBoxCharacter::ApplyDamage(float DamageAmount, AActor* DamageCauser)
{
	float CurrentHP = GetCurrentlyHP();
	CurrentHP -= DamageAmount;
	SetCurrentlyHP(CurrentHP);
}

