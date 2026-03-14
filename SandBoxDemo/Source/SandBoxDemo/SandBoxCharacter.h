// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Peoples/SandBoxPeople_King.h"
#include "EnhancedInputSubsystems.h"
#include "SandBoxPeople.h"
#include "Component/SandBoxMovementComponent.h"
#include "Datas/CharacterDataAsset.h"
#include "Attributes/AttributeBase.h"
#include "State/StateMachineBase.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "SandBoxCharacter.generated.h"
class ASandBox_Prop;
class UUI_CharacterInfo;
UENUM(BlueprintType)
 enum EAttackType : uint8
{
	AT_None,
	AT_CloseAttack,
	AT_FarAttack
};



UCLASS()
class SANDBOXDEMO_API ASandBoxCharacter : public ACharacter
{
	
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASandBoxCharacter();
/************************************技能***************************************************/
protected:
	//技能组件
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	//激活技能
	bool ActivateAbilityByTag(FGameplayTag AbilityTag);
	//初始化技能
	void InitalAbility();

/************************************属性***************************************************/
private:
	//角色属性配置表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterDataAsset> CharacterDataAsset;
	//初始化属性
	void InitAttribute();

protected:
	//设置属性
	UFUNCTION()
	bool SetAttributeByEnum(ECharacterAttribute Attribute, float NewValue, ECAVType InValueType =ECAVType::CAVT_Currently);
	//服务器设置属性值
	UFUNCTION(Server,Reliable)
	void Server_SetAttributeByEnum(ECharacterAttribute Attribute, float NewValue, ECAVType InValueType = ECAVType::CAVT_Currently);
	//服务器设置属性值
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetAttributeByEnum(ECharacterAttribute Attribute, float NewValue, ECAVType InValueType = ECAVType::CAVT_Currently);

	
	//获取属性
	UFUNCTION(BlueprintCallable)
	float GetAttributeByEnum(ECharacterAttribute Attribute, ECAVType InValueType = ECAVType::CAVT_Currently);
public:
	//设置血量
	UFUNCTION()
	bool SetCurrentlyHP(float NewHP);
	//获取血量
	UFUNCTION()
	float GetCurrentlyHP();
/************************************组件***************************************************/
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStateMachineBase> StateMachine;

/************************************运动***************************************************/
protected:
	EMovementMode MovementMode;

protected:
	//设置角色运动类型
	UFUNCTION(BlueprintCallable)
	void SetCurrentlyMoveMode(EMovementMode InMoveState);
private:
	UFUNCTION(Server,Reliable)
	void Server_SetCurrentlyMoveMode(EMovementMode InMoveState);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_SetCurrentlyMoveMode(EMovementMode InMoveState);

protected:
	//获取角色运动类型
	UFUNCTION(BlueprintCallable,BlueprintPure)
	EMovementMode GetCurrentlyMoveMode();
	//当角色运动改变
	UFUNCTION()
	virtual void OnMoveModeChange();
public:
	//设置左右旋转速度
	bool SetCurrentlyYawRotatorSpeed(float InRotatorSpeed);
	//获取左右转向速度
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentlyYawRotatorSpeed();

	//设置最大左右旋转速度
	bool SetMaxYawRotatorSpeed(float InRotatorSpeed);
	//获取最大左右转向速度
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxYawRotatorSpeed();

	//设置上下旋转速度
	bool SetCurrentlyPitchRotatorSpeed(float InRotatorSpeed);
	//获取上下旋转速度
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentlyPitchRotatorSpeed();

	//设置最大上下旋转速度
	bool SetMaxPitchRotatorSpeed(float InRotatorSpeed);
	//获取最大上下旋转速度
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxPitchRotatorSpeed();

	//设置最大移动速度
	void SetMaxMoveSpeed(float InSpeed);
private:
	//设置最大移动速度
	UFUNCTION(Server,Reliable)
	void Server_SetMaxMoveSpeed(float InSpeed);
	//设置最大移动速度
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_SetMaxMoveSpeed(float InSpeed);
public:

	//获取最大移动速度
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxQuickMoveSpeed();
	//获取最大移动速度
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxMoveSpeed();

	//设置当前移动速度
	bool SetCurrentlyMoveSpeed(float InSpeed);
	//获取当前移动速度
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentlyMoveSpeed();

/************************************相机***************************************************/
protected:
	//相机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;
	//相机弹簧臂
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;
	//角色的控制点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* CharacterControlPoint;

/************************************输入绑定***************************************************/
	//输入动作
protected:
	//输入上下文
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;
	//移动输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	//近战攻击输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CloseAttackAction;
	//远程攻击输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FarAttackAction;
	//查看输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	//跳跃
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	//起飞
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TakeOffAction;
	//着陆
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LandAction;
	//开始跑
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RunAction;
	//交互
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

protected:
	//近战攻击
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void CloseAttack();
	//远程攻击
	UFUNCTION(BlueprintCallable, Category = "Attack|FarAttack")
	void FarAttack();
	//移动
	UFUNCTION()
	void Move(const FInputActionValue& InputValue);

	void WalkMove(const FInputActionValue& InputValue);

	void FlyMove(const FInputActionValue& InputValue);

	void StopMove(const FInputActionValue& InputValue);

	void StartJump();

	void StartQuick();

	void StopQuick();

	void Interact();

	//起飞
	UFUNCTION()
	void TakeOff(const FInputActionValue& InputValue);

	void Land(const FInputActionValue& InputValue);

	//视野
	UFUNCTION()
	void Look(const FInputActionValue& InputValue);
public:
		UFUNCTION(BlueprintCallable)
		void StartSwim();

		UFUNCTION(BlueprintCallable)
		void StopSwim();
/************************************旋转，方向相关***************************************************/
public:

	//获取Actor向前向量
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCharacterRightVector();
	//获取Actor右向量
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCharacterForwardVector();
	//获取Actor向上前向量
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCharacterUpVector();
	//获取当前角色旋转
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FRotator GetCharacterRotation();
	
/************************************Tick相关***************************************************/
private:

	//更新属性
	void UpdateAttributes(float DeltaTime);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



/************************************玩家状态类***************************************************/
//todo
private:
	//更新角色基本状态
	void UpdateCharacterState(float DeltaTime);
public:	
	//获取当前的角色状态
	UFUNCTION(BlueprintCallable,BlueprintPure)
	TEnumAsByte<EState> GetCurrentlyChracterState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void BeHit(float Damage);
	//玩家阵营
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	EPeopleCamp CurrentCamp;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
