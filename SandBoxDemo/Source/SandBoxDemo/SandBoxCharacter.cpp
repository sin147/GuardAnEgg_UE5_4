// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Props/SandBox_Prop.h"
#include "UI/UI_CharacterInfo.h"
#include "Components/ArrowComponent.h"
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
	SpringArmComponent->SetupAttachment(RootComponent);
	//相机组件
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	//角色控制点
	CharacterControlPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterControlPoint"));
	CharacterControlPoint->SetupAttachment(RootComponent);
	GetMesh()->SetupAttachment(CharacterControlPoint);
	//初始化当前Yaw和Pitch用于计算旋转速度
	PreYaw = GetCharacterRotation().Yaw;
	PrePitch = GetCharacterRotation().Pitch;
}

void ASandBoxCharacter::InitAttribute()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	//工具数据资产初始化属性
	//飞行速度
	CharacterMovementComponent->MaxFlySpeed = (*AttributeDataAsset)[ECharacterAttribute::FlySpeed].GetMaxValue();
	//步行速度
	CharacterMovementComponent->MaxWalkSpeed = (*AttributeDataAsset)[ECharacterAttribute::WalkSpeed].GetMaxValue();
	//游泳速度
	CharacterMovementComponent->MaxSwimSpeed = (*AttributeDataAsset)[ECharacterAttribute::SwimmingSpeed].GetMaxValue();
	//左右旋转速度
	CharacterMovementComponent->RotationRate.Yaw = (*AttributeDataAsset)[ECharacterAttribute::YawRotatorSpeed].GetMaxValue();
	//上下旋转速度
	CharacterMovementComponent->RotationRate.Pitch = (*AttributeDataAsset)[ECharacterAttribute::PitchRotatorSpeed].GetMaxValue();
}

bool ASandBoxCharacter::SetAttributeByEnum(ECharacterAttribute Attribute, float NewValue,ECAVType InValueType)
{
	if (!AttributeDataAsset->IsVaildKey(Attribute)){ return false; }
	switch (InValueType)
	{
	case CAVT_Max:
		 (*AttributeDataAsset)[Attribute].SetMaxValue(NewValue);
		break;
	case CAVT_Currently:
		(*AttributeDataAsset)[Attribute].SetCurrentlyValue(NewValue);
		break;
	case CAVT_Min:
		(*AttributeDataAsset)[Attribute].SetMinValue(NewValue);
		break;
	default:
		return false;
	}
	return true;
}

float ASandBoxCharacter::GetAttributeByEnum(ECharacterAttribute Attribute, ECAVType InValueType)
{

	if (!AttributeDataAsset->IsVaildKey(Attribute)) { return 0; }
	switch (InValueType)
	{
	case CAVT_Max:
		return (*AttributeDataAsset)[Attribute].GetMaxValue();
	case CAVT_Currently:
		return (*AttributeDataAsset)[Attribute].GetCurrentlyValue();
	case CAVT_Min:
		return (*AttributeDataAsset)[Attribute].GetMinValue();
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
	if (GetCurrentlyMoveSpeed() != 0)
	{
		StateMachine->EnterState(EState::S_Move);
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
	//更新速度
	SetCurrentlyMoveSpeed(GetVelocity().Length());
	//更新左右旋转速度
	SetCurrentlyYawRotatorSpeed((GetCharacterRotation().Yaw - PreYaw)/DeltaTime);
	//UE_LOG(LogTemp, Log, TEXT("YawRotatorSpeed %lf %lf %lf "), GetCharacterRotation().Yaw,PreYaw,(GetCharacterRotation().Yaw - PreYaw) / DeltaTime);
	PreYaw = GetCharacterRotation().Yaw;

	//更新上下速度旋转
	SetCurrentlyPitchRotatorSpeed((GetCharacterRotation().Pitch - PrePitch) / DeltaTime);
	//UE_LOG(LogTemp, Log, TEXT("PitchRotatorSpeed %lf %lf %lf "), GetCharacterRotation().Pitch, PrePitch, (GetCharacterRotation().Pitch - PrePitch) / DeltaTime);
	PrePitch = GetCharacterRotation().Pitch;
}

bool ASandBoxCharacter::SetCurrentlyYawRotatorSpeed(float InRotatorSpeed)
{

	return SetAttributeByEnum(ECharacterAttribute::YawRotatorSpeed, InRotatorSpeed);
}
float ASandBoxCharacter::GetCurrentlyYawRotatorSpeed()
{
	//UE_LOG(LogTemp, Log, TEXT("YawRotatorSpeed %lf "), GetAttributeByEnum(ECharacterAttribute::YawRotatorSpeed));
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
	//UE_LOG(LogTemp, Log, TEXT("PitchRotatorSpeed %lf "), GetAttributeByEnum(ECharacterAttribute::PitchRotatorSpeed));
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

bool ASandBoxCharacter::SetMaxMoveSpeed(float InSpeed)
{
	switch (GetCharacterMovement()->MovementMode)
	{
	case MOVE_Walking:
		return SetAttributeByEnum(ECharacterAttribute::WalkSpeed, InSpeed,ECAVType::CAVT_Max);
	case MOVE_Swimming:
		return SetAttributeByEnum(ECharacterAttribute::SwimmingSpeed, InSpeed, ECAVType::CAVT_Max);
	case MOVE_Flying:
		return SetAttributeByEnum(ECharacterAttribute::FlySpeed, InSpeed, ECAVType::CAVT_Max);
	default:
		return false;
	}
}

float ASandBoxCharacter::GetMaxMoveSpeed()
{
	switch (GetCharacterMovement()->MovementMode)
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

bool ASandBoxCharacter::SetCurrentlyMoveSpeed(float InSpeed)
{
	switch (GetCharacterMovement()->MovementMode)
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
	switch (GetCharacterMovement()->MovementMode)
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
	GetCharacterMovement()->SetMovementMode(InMoveState);
}

EMovementMode ASandBoxCharacter::GetCurrentlyMoveMode()
{
	return GetCharacterMovement()->MovementMode;
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
	FVector InputValue = Value.Get<FVector>();
	if (!IsValid(Controller)) { return; }
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	const FRotator ControllerRotation = Controller->GetControlRotation();
	const FRotator ControllerYawRotation(0, ControllerRotation.Yaw, 0);
	const FRotator ControllerPitchRotation(ControllerRotation.Pitch,0, 0);
	const FRotator CharacterRotation= GetCharacterRotation();
	const FRotator CharacterYawRotation(0, CharacterRotation.Yaw, 0);
	const FRotator CharacterPitchRotation(0,0, CharacterRotation.Pitch);
	float PitchLimit=90;
	// 应用前进和后退输入
	if (InputValue.X != 0)
	{
		//设置上下偏转限制
		PitchLimit = 60;
		const FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, InputValue.X);
	}

	//左右旋转
	if (InputValue.Y != 0&& ControllerYawRotation.Equals(CharacterYawRotation,60))
	{
		AddControllerYawInput(InputValue.Y);
		if (InputValue.X == 0)
		{
			//飞行状态下的左右旋转速度
			if (GetCharacterMovement()->RotationRate.Yaw != 500)
			{
				GetCharacterMovement()->RotationRate.Yaw = 500;
			}
			const FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(ForwardDirection, 0.0001);

		}
		//非飞行状态下的旋转速度
		else if(GetCharacterMovement()->RotationRate.Yaw==500)
		{
			GetCharacterMovement()->RotationRate.Yaw = 100;
		}

	}
	//左右旋转取消，清除控制器旋转差量
	else if(!ControllerRotation.Equals(CharacterRotation,10))
	{
		Controller->SetControlRotation(FRotator(ControllerRotation.Pitch, CharacterRotation.Yaw, ControllerRotation.Roll));
	}

	//上下旋转
	if (InputValue.Z != 0&& ControllerPitchRotation.Equals(FRotator(0, 0, 0), PitchLimit))
	{
		if (InputValue.X == 0)
		{
			AddControllerPitchInput(InputValue.Z);
			const FVector ForwardDirection = FRotationMatrix(ControllerRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(ForwardDirection, 1);
		}
		AddControllerPitchInput(InputValue.Z);
	}
	//超过限制回归到限制范围
	else
	{
		if (!ControllerPitchRotation.Equals(FRotator(PitchLimit* -InputValue.Z, 0, 0), 10))
		{
			Controller->SetControlRotation(FRotator(PitchLimit * -InputValue.Z, ControllerRotation.Yaw, ControllerRotation.Roll));
		}

	}
}

void ASandBoxCharacter::StopMove(const FInputActionValue& InputValue)
{
	/*const FRotator ControllerRotation = Controller->GetControlRotation();
	const FRotator CharacterRotation = FRotator(GetMesh()->GetComponentRotation().Pitch, GetMesh()->GetComponentRotation().Yaw + 90, GetMesh()->GetComponentRotation().Roll);
	Controller->SetControlRotation(FRotator(0, CharacterRotation.Yaw, CharacterRotation.Roll));*/
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

FVector ASandBoxCharacter::GetCharacterForwardVector()
{
	return CharacterControlPoint->GetRightVector();
}

FVector ASandBoxCharacter::GetCharacterRightVector()
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
	//更新速度
	UpdateAttributes(DeltaTime);
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

