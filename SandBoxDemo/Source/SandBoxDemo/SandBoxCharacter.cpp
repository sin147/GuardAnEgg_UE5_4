// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Props/SandBox_Prop.h"
#include "UI/UI_CharacterInfo.h"
#include "Components/ArrowComponent.h"
#include "GameplayTagContainer.h"
#include "SandBoxPlayerController.h"
#include "System/InteractiveSubsystem.h"
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
		CharacterMovementComponent->MaxFlySpeed = CharacterDataAsset->GetAttributeByEnum(ECharacterAttribute::FlySpeed).GetMaxValue()* CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed;
		//步行速度
		CharacterMovementComponent->MaxWalkSpeed = CharacterDataAsset->GetAttributeByEnum(ECharacterAttribute::WalkSpeed).GetMaxValue() * CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed;
		//游泳速度
		CharacterMovementComponent->MaxSwimSpeed = CharacterDataAsset->GetAttributeByEnum(ECharacterAttribute::SwimmingSpeed).GetMaxValue() * CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed;
	}
	if (CharacterDataAsset and SandBoxPlayerController)
	{
		//左右旋转速度
		SandBoxPlayerController->MaxRotationRate.Yaw = CharacterDataAsset->GetAttributeByEnum(ECharacterAttribute::YawRotatorSpeed).GetMaxValue();
		//上下旋转速度
		SandBoxPlayerController->MaxRotationRate.Pitch = CharacterDataAsset->GetAttributeByEnum(ECharacterAttribute::PitchRotatorSpeed).GetMaxValue();
	}
	SetCurrentlyMoveMode(EMovementMode::MOVE_Walking);

}

bool ASandBoxCharacter::SetAttributeByEnum(ECharacterAttribute InAttribute, float InNewValue, ECAVType InValueType)
{
	if (!IsValid(CharacterDataAsset) or !CharacterDataAsset->IsVaildAttribute(InAttribute))
	{
		return false;
	}
	//if (InAttribute == ECharacterAttribute::YawRotatorSpeed)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("%lf"), InNewValue);
	//}

	Server_SetAttributeByEnum(InAttribute, InNewValue,InValueType);
	return true;
}

void ASandBoxCharacter::Server_SetAttributeByEnum_Implementation(ECharacterAttribute InAttribute, float InNewValue, ECAVType InValueType)
{
	if (!IsValid(CharacterDataAsset) or !CharacterDataAsset->IsVaildAttribute(InAttribute)) { return; }
	switch (InValueType)
	{
	case CAVT_Max:
		CharacterDataAsset->GetAttributeByEnum(InAttribute).SetMaxValue(InNewValue);
		break;
	case CAVT_Currently:
		CharacterDataAsset->GetAttributeByEnum(InAttribute).SetCurrentlyValue(InNewValue);
		break;
	case CAVT_Min:
		CharacterDataAsset->GetAttributeByEnum(InAttribute).SetMinValue(InNewValue);
		break;
	default:
		break;
	}
	Multicast_SetAttributeByEnum(InAttribute, InNewValue,  InValueType);
	//if (InAttribute == ECharacterAttribute::YawRotatorSpeed)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("%lf"), InNewValue);
	//}
}

void ASandBoxCharacter::Multicast_SetAttributeByEnum_Implementation(ECharacterAttribute InAttribute, float InNewValue, ECAVType InValueType)
{
	if (!IsValid(CharacterDataAsset) or !CharacterDataAsset->IsVaildAttribute(InAttribute)) { return; }
	//if (InAttribute == ECharacterAttribute::YawRotatorSpeed)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("%lf"), InNewValue);
	//}
	switch (InValueType)
	{
	case CAVT_Max:
		CharacterDataAsset->GetAttributeByEnum(InAttribute).SetMaxValue(InNewValue);
		break;
	case CAVT_Currently:
		if (InAttribute == ECharacterAttribute::WalkSpeed)
{
		//UE_LOG(LogTemp, Log, TEXT("%lf"), InNewValue);
}
		CharacterDataAsset->GetAttributeByEnum(InAttribute).SetCurrentlyValue(InNewValue);
		break;
	case CAVT_Min:
		CharacterDataAsset->GetAttributeByEnum(InAttribute).SetMinValue(InNewValue);
		break;
	default:
		break;
	}
}

float ASandBoxCharacter::GetAttributeByEnum(ECharacterAttribute Attribute, ECAVType InValueType)
{

	if (!IsValid(CharacterDataAsset) or !CharacterDataAsset->IsVaildAttribute(Attribute)) { return 0; }
	switch (InValueType)
	{
	case CAVT_Max:
		return CharacterDataAsset->GetAttributeByEnum(Attribute).GetMaxValue();
	case CAVT_Currently:
		//if (Attribute == ECharacterAttribute::YawRotatorSpeed)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("%lf"), CharacterDataAsset->GetAttributeByEnum(Attribute).GetCurrentlyValue());
		//}
		return CharacterDataAsset->GetAttributeByEnum(Attribute).GetCurrentlyValue();
	case CAVT_Min:
		return CharacterDataAsset->GetAttributeByEnum(Attribute).GetMinValue();
	default:
		return 0;
	}

}

bool ASandBoxCharacter::SetCurrentlyHP(float NewHP)
{
	return SetAttributeByEnum(ECharacterAttribute::HP, NewHP);
}

float ASandBoxCharacter::GetCurrentlyHP()
{
	return GetAttributeByEnum(ECharacterAttribute::HP);
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
		if (GetAttributeByEnum(ECharacterAttribute::EnduranceValue) <= 0)
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

	//快速移动的体力消耗
	if (StateMachine->GetCurrentlyState() != EState::S_QuickMove)
	{
		SetAttributeByEnum(ECharacterAttribute::EnduranceValue, GetAttributeByEnum(ECharacterAttribute::EnduranceValue) + GetAttributeByEnum(ECharacterAttribute::EnduranceValue,ECAVType::CAVT_Max) *CharacterDataAsset->QuickMoveEnduranceConsumptionValue * DeltaTime);
	}
	else
	{
		SetAttributeByEnum(ECharacterAttribute::EnduranceValue, GetAttributeByEnum(ECharacterAttribute::EnduranceValue) - GetAttributeByEnum(ECharacterAttribute::EnduranceValue, ECAVType::CAVT_Max) * CharacterDataAsset->QuickMoveEnduranceConsumptionValue * DeltaTime * (AbsMoveSpeed / GetMaxQuickMoveSpeed()));
	}

	//UE_LOG(LogTemp, Log, TEXT("MoveSpeed:%lf"), GetSpeed());
}

TEnumAsByte<EState> ASandBoxCharacter::GetCurrentlyChracterState()
{

	return StateMachine->GetCurrentlyState();
}

void ASandBoxCharacter::UpdateAttributes(float DeltaTime)
{
	//更新速度
	switch (MovementMode)
	{
	case MOVE_Swimming:
	case MOVE_Flying:
		SetCurrentlyMoveSpeed(FVector::DotProduct(GetVelocity(), GetCharacterForwardVector()));
		//UE_LOG(LogTemp, Log, TEXT("%lf"), FVector::DotProduct(GetVelocity(), GetCharacterForwardVector()));
		break;
	default:
		SetCurrentlyMoveSpeed(FVector::DotProduct(FVector(GetVelocity().X, GetVelocity().Y, 0), GetCharacterForwardVector()));
		//UE_LOG(LogTemp, Log, TEXT("%lf"), FVector::DotProduct(FVector(GetVelocity().X, GetVelocity().Y, 0), GetCharacterForwardVector()));
		break;
	}
	ASandBoxPlayerController* PlayController = GetController<ASandBoxPlayerController>();
	if (PlayController)
	{
		//更新左右旋转速度
		SetCurrentlyYawRotatorSpeed(PlayController->GetRotationRate().Yaw);
		//UE_LOG(LogTemp, Log, TEXT("%lf"), PlayController->GetRotationRate().Yaw);
		//更新上下速度旋转
		SetCurrentlyPitchRotatorSpeed(PlayController->GetRotationRate().Pitch);
	}

}

bool ASandBoxCharacter::SetCurrentlyYawRotatorSpeed(float InRotatorSpeed)
{
	if (SetAttributeByEnum(ECharacterAttribute::YawRotatorSpeed, InRotatorSpeed))
	{
		return true;
	}
	else
	{
		return false;
	}

}
float ASandBoxCharacter::GetCurrentlyYawRotatorSpeed()
{
	return GetAttributeByEnum(ECharacterAttribute::YawRotatorSpeed);
}
bool ASandBoxCharacter::SetMaxYawRotatorSpeed(float InRotatorSpeed)
{
	return SetAttributeByEnum(ECharacterAttribute::YawRotatorSpeed, InRotatorSpeed, ECAVType::CAVT_Max);
}
float ASandBoxCharacter::GetMaxYawRotatorSpeed()
{
	return GetAttributeByEnum(ECharacterAttribute::YawRotatorSpeed, ECAVType::CAVT_Max);
}

bool ASandBoxCharacter::SetCurrentlyPitchRotatorSpeed(float InRotatorSpeed)
{
	return SetAttributeByEnum(ECharacterAttribute::PitchRotatorSpeed, InRotatorSpeed);
}
float ASandBoxCharacter::GetCurrentlyPitchRotatorSpeed()
{
	return GetAttributeByEnum(ECharacterAttribute::PitchRotatorSpeed);
}

bool ASandBoxCharacter::SetMaxPitchRotatorSpeed(float InRotatorSpeed)
{
	return SetAttributeByEnum(ECharacterAttribute::PitchRotatorSpeed,InRotatorSpeed,ECAVType::CAVT_Max);
}

float ASandBoxCharacter::GetMaxPitchRotatorSpeed()
{
	
	return GetAttributeByEnum(ECharacterAttribute::PitchRotatorSpeed,ECAVType::CAVT_Max);
}

void ASandBoxCharacter::SetMaxMoveSpeed(float InSpeed)
{
	Server_SetMaxMoveSpeed(InSpeed);
}

void ASandBoxCharacter::Server_SetMaxMoveSpeed_Implementation(float InSpeed)
{
	switch (MovementMode)
	{
	case MOVE_Walking:
		GetCharacterMovement()->MaxWalkSpeed = InSpeed;
		break;
	case MOVE_Swimming:
		GetCharacterMovement()->MaxSwimSpeed = InSpeed;
		break;
	case MOVE_Flying:
		GetCharacterMovement()->MaxFlySpeed = InSpeed;
		break;
	default:
		return;
	}
	Multicast_SetMaxMoveSpeed(InSpeed);
}

void ASandBoxCharacter::Multicast_SetMaxMoveSpeed_Implementation(float InSpeed)
{
	switch (MovementMode)
	{
	case MOVE_Walking:
		GetCharacterMovement()->MaxWalkSpeed = InSpeed;
		break;
	case MOVE_Swimming:
		GetCharacterMovement()->MaxSwimSpeed = InSpeed;
		break;
	case MOVE_Flying:
		GetCharacterMovement()->MaxFlySpeed = InSpeed;
		break;
	default:
		return;
	}
}

float ASandBoxCharacter::GetMaxQuickMoveSpeed()
{
	switch (MovementMode)
	{
	case MOVE_Walking:
		return GetAttributeByEnum(ECharacterAttribute::WalkSpeed, ECAVType::CAVT_Max);
	case MOVE_Swimming:
		return GetAttributeByEnum(ECharacterAttribute::SwimmingSpeed, ECAVType::CAVT_Max);
	case MOVE_Flying:
		return GetAttributeByEnum(ECharacterAttribute::FlySpeed, ECAVType::CAVT_Max);
	default:
		return 0;
	}
}

float ASandBoxCharacter::GetMaxNormalMoveSpeed()
{
	float MoveSpeed;
	switch (MovementMode)
	{
	case MOVE_Walking:
		MoveSpeed= GetAttributeByEnum(ECharacterAttribute::WalkSpeed, ECAVType::CAVT_Max) * CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed;
		break;
	case MOVE_Swimming:
		MoveSpeed= GetAttributeByEnum(ECharacterAttribute::SwimmingSpeed, ECAVType::CAVT_Max) * CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed;
		break;
	case MOVE_Flying:
		MoveSpeed= GetAttributeByEnum(ECharacterAttribute::FlySpeed, ECAVType::CAVT_Max) * CharacterDataAsset->QuickMoveSpeedRateByMaxMoveSpeed;
		break;
	default:
		return 0;
	}
	return MoveSpeed;
}

bool ASandBoxCharacter::SetCurrentlyMoveSpeed(float InSpeed)
{
	switch (MovementMode)
	{
	case MOVE_Walking:
		return SetAttributeByEnum(ECharacterAttribute::WalkSpeed, InSpeed);
	case MOVE_Swimming:
		return SetAttributeByEnum(ECharacterAttribute::SwimmingSpeed, InSpeed);
	case MOVE_Flying:
		return SetAttributeByEnum(ECharacterAttribute::FlySpeed, InSpeed);
	default:
		return false;
	}
}

float ASandBoxCharacter::GetCurrentlyMoveSpeed()
{
	switch (MovementMode)
	{
	case MOVE_Walking:
		return GetAttributeByEnum(ECharacterAttribute::WalkSpeed);
	case MOVE_Swimming:
		return GetAttributeByEnum(ECharacterAttribute::SwimmingSpeed);
	case MOVE_Flying:
		return GetAttributeByEnum(ECharacterAttribute::FlySpeed);
	default:
		return 0;
	}
}


void ASandBoxCharacter::SetCurrentlyMoveMode(EMovementMode InMoveState)
{
	Server_SetCurrentlyMoveMode_Implementation(InMoveState);
}

void ASandBoxCharacter::Server_SetCurrentlyMoveMode_Implementation(EMovementMode InMoveState)
{
	MovementMode = InMoveState;
	SetMaxMoveSpeed(GetMaxNormalMoveSpeed());
	GetCharacterMovement()->SetMovementMode(InMoveState);
	Multicast_SetCurrentlyMoveMode(InMoveState);
}

void ASandBoxCharacter::Multicast_SetCurrentlyMoveMode_Implementation(EMovementMode InMoveState)
{
	MovementMode = InMoveState;
	SetMaxMoveSpeed(GetMaxNormalMoveSpeed());
	GetCharacterMovement()->SetMovementMode(InMoveState);
}

EMovementMode ASandBoxCharacter::GetCurrentlyMoveMode()
{
	return MovementMode;
}

void ASandBoxCharacter::OnMoveModeChange()
{
	//UE_LOG(LogTemp, Log, TEXT("%s : Move State Change To %d"), *GetActorNameOrLabel(), GetCurrentlyMoveMode());
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
	switch (MovementMode)
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
	//GetCharacterMovement()->MaxAcceleration = 0;
	SetMaxMoveSpeed(GetMaxQuickMoveSpeed());
}

void ASandBoxCharacter::StopQuick()
{
	//GetCharacterMovement()->MaxAcceleration = 2048;
	SetMaxMoveSpeed(GetMaxNormalMoveSpeed());
}

void ASandBoxCharacter::Interact()
{
	GetGameInstance()->GetSubsystem<UInteractiveSubsystem>()->RequestInteract(this);
}

void ASandBoxCharacter::StartSwim()
{
	SetCurrentlyMoveMode(EMovementMode::MOVE_Swimming);
}

void ASandBoxCharacter::StopSwim()
{
	if (GetCurrentlyMoveMode() == EMovementMode::MOVE_Swimming)
	{
		SetCurrentlyMoveMode(EMovementMode::MOVE_Walking);
	}

}

void ASandBoxCharacter::TakeOff(const FInputActionValue& InputValue)
{
	//起飞逻辑
	ActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.TakeOff")));
}

void ASandBoxCharacter::Land(const FInputActionValue& InputValue)
{
	ActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Land")));
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

bool ASandBoxCharacter::ApplyDamage(float DamageAmount, AActor* DamageCauser)
{
	float CurrentHP = GetCurrentlyHP();
	CurrentHP -= DamageAmount;
	return SetCurrentlyHP(CurrentHP);
}

