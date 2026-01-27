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
	CurrentlyYaw = GetMesh()->GetComponentRotation().Yaw;
	//箭头附加到Mesh上
	//GetArrowComponent()->SetupAttachment(GetMesh());
}

void ASandBoxCharacter::InitAttribute(TObjectPtr<UCharacterAttributeDataAsset> InAttributeDataAsset)
{
	TArray<FCharacterAttribute> AttributeDatas = InAttributeDataAsset->Attributes;
	for (FCharacterAttribute AttributeData : AttributeDatas)
	{
		TObjectPtr<UAttributeBase> NewAttribute = NewObject<UAttributeBase>();
		NewAttribute->Init(AttributeData.MaxValue, AttributeData.MinValue, AttributeData.Default);
		CharacterAtributes.Add(AttributeData.Attribute, NewAttribute);
	}
}

bool ASandBoxCharacter::SetAttributeByEnum(TEnumAsByte<ECharacterAttribute> Attribute, float NewValue)
{
	if (CharacterAtributes.Find(Attribute))
	{
		CharacterAtributes[Attribute]->SetCurrentlyValue(NewValue);
	}
	else
	{
		TObjectPtr<UAttributeBase> NewAttribute= NewObject<UAttributeBase>();
		CharacterAtributes.Add(Attribute, NewAttribute);
		UE_LOG(LogTemp, Log, TEXT("Not Find Attribute:%d,New One"), Attribute.GetIntValue());
	}
	return true;
}

float ASandBoxCharacter::GetAttributeByEnum(TEnumAsByte<ECharacterAttribute> Attribute)
{

	if (!CharacterAtributes.Find(Attribute))
	{
		TObjectPtr<UAttributeBase> NewAttribute = NewObject<UAttributeBase>();
		CharacterAtributes.Add(Attribute, NewAttribute);
		UE_LOG(LogTemp, Log, TEXT("Not Find Attribute:%d,New One"), Attribute.GetIntValue());
	}
	return CharacterAtributes[Attribute]->GetCurrentlyValue();
}

void ASandBoxCharacter::UpdateCharacterState(float DeltaTime)
{
	if (GetSpeed() != 0)
	{
		StateMachine->EnterState(EState::S_Move);

	}
	else
	{
		StateMachine->EnterState(EState::S_Idle);
	}
	UE_LOG(LogTemp, Log, TEXT("MoveSpeed:%lf"), GetSpeed());
}

TEnumAsByte<EState> ASandBoxCharacter::GetCurrentlyChracterState()
{

	return StateMachine->GetCurrentlyState();
}

void ASandBoxCharacter::UpdateAttributes(float DeltaTime)
{
	//更新速度
	SetSpeed(GetVelocity().Length());
	//更新角色旋转速度
	SetRotatorSpeed((GetMesh()->GetComponentRotation().Yaw - CurrentlyYaw) / DeltaTime);
	//UE_LOG(LogTemp, Log, TEXT("%lf : Close Attack"), GetMesh()->GetComponentRotation().Yaw - CurrentlyYaw);
	CurrentlyYaw = GetMesh()->GetComponentRotation().Yaw;

}

float ASandBoxCharacter::GetSpeed()
{
	return GetAttributeByEnum(ECharacterAttribute::MoveSpeed);
}

void ASandBoxCharacter::SetCurrentlyMoveMode(EMovementMode InMoveState)
{
	GetCharacterMovement()->SetMovementMode(InMoveState);
	OnMoveModeChange();
}

EMovementMode ASandBoxCharacter::GetCurrentlyMoveMode()
{
	return GetCharacterMovement()->MovementMode;
}

void ASandBoxCharacter::OnMoveModeChange()
{
	UE_LOG(LogTemp, Log, TEXT("%s : Move State Change To %d"), *GetActorNameOrLabel(), GetCurrentlyMoveMode());
}

void ASandBoxCharacter::CloseAttack()
{
	UE_LOG(LogTemp, Log, TEXT("%s : Close Attack"),*GetActorNameOrLabel());
}

void ASandBoxCharacter::FarAttack()
{
	UE_LOG(LogTemp, Log, TEXT("%s : Far Attack"), *GetActorNameOrLabel());
}

void ASandBoxCharacter::Move(const FInputActionValue& Value)
{
	FVector InputValue = Value.Get<FVector>();
	 AddMovementInput(GetCharacterForwardVector(), InputValue.X);
	if (InputValue.X<0)
	{
		GetMesh()->SetWorldRotation(FRotator(GetMesh()->GetComponentRotation().Pitch, GetMesh()->GetComponentRotation().Yaw - InputValue.Y, GetMesh()->GetComponentRotation().Roll));
	}
	else
	{
		GetMesh()->SetWorldRotation(FRotator(GetMesh()->GetComponentRotation().Pitch, GetMesh()->GetComponentRotation().Yaw + InputValue.Y, GetMesh()->GetComponentRotation().Roll));
	}

	AddMovementInput(GetActorUpVector(), InputValue.Z);
	//UE_LOG(LogTemp, Log, TEXT("Move: %s"), *InputValue.ToString());
}

void ASandBoxCharacter::Look(const FInputActionValue& Value)
{
	//旋转相机臂
	FVector2D InputValue = Value.Get<FVector2D>();
	FRotator NewRotator = SpringArmComponent->GetComponentRotation();
	NewRotator.Pitch = UKismetMathLibrary::ClampAngle(NewRotator.Pitch + InputValue.Y, -80.f, 80.f);
	NewRotator.Yaw += InputValue.X;
	SpringArmComponent->SetWorldRotation(NewRotator);
	//UE_LOG(LogTemp, Log, TEXT("Look: %s"),*SpringArmComponent->GetComponentRotation().ToString());
}

FVector ASandBoxCharacter::GetCharacterForwardVector()
{
	return GetMesh()->GetRightVector();
}

FVector ASandBoxCharacter::GetCharacterRightVector()
{
	return GetMesh()->GetForwardVector();
}

FVector ASandBoxCharacter::GetCharacterUpVector()
{
	return GetMesh()->GetUpVector();
}

// Called when the game starts or when spawned
void ASandBoxCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAttribute(AttributeDataAsset);
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

