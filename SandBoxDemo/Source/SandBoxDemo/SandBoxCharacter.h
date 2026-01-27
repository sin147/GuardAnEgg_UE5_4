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
#include "GameFramework/CharacterMovementComponent.h"
#include "Datas/CharacterAttributeDataAsset.h"
#include "Attributes/AttributeBase.h"
#include "State/StateMachineBase.h"
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
/************************************属性***************************************************/
private:
	//角色属性配置表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterAttributeDataAsset> AttributeDataAsset;
	//校色属性
	TMap<TEnumAsByte<ECharacterAttribute>, TObjectPtr<UAttributeBase>> CharacterAtributes;
	//初始化属性
	void InitAttribute(TObjectPtr<UCharacterAttributeDataAsset> InAttributeDataAsset);

protected:
	//设置属性
	UFUNCTION()
	bool SetAttributeByEnum(TEnumAsByte<ECharacterAttribute>Attribute, float NewValue);
	//获取属性
	UFUNCTION()
	float GetAttributeByEnum(TEnumAsByte<ECharacterAttribute>Attribute);
public:
	//设置血量
	UFUNCTION()
	bool SetHP(float NewHP)
	{
		return SetAttributeByEnum(ECharacterAttribute::HP,NewHP);
	}
	//获取血量
	UFUNCTION()
	float GetHP()
	{
		return GetAttributeByEnum(ECharacterAttribute::HP);
	}
	//设置移动速度
	bool SetSpeed(float InSpeed)
	{
		return SetAttributeByEnum(ECharacterAttribute::MoveSpeed, InSpeed);
	}
	//获取移动速度
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpeed();
	//获取最大移动速度

	//获取转向速度
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetRotatorSpeed()
	{
		return GetAttributeByEnum(ECharacterAttribute::RotatorSpeed);
	}
	//设置旋转速度
	bool SetRotatorSpeed(float InRotatorSpeed)
	{
		return SetAttributeByEnum(ECharacterAttribute::RotatorSpeed, InRotatorSpeed);
	}
/************************************组件***************************************************/
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStateMachineBase> StateMachine;
/************************************运动***************************************************/
protected:
	//设置角色运动类型
	void SetCurrentlyMoveMode(EMovementMode InMoveState);
	//获取角色运动类型
	UFUNCTION(BlueprintCallable,BlueprintPure)
	EMovementMode GetCurrentlyMoveMode();
	//当角色运动改变
	UFUNCTION()
	virtual void OnMoveModeChange();

/************************************相机***************************************************/
protected:
	//相机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;
	//相机弹簧臂
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;
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
	//视野
	UFUNCTION()
	void Look(const FInputActionValue& InputValue);
/************************************方向相关***************************************************/
protected:

	//获取Actor向前向量
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCharacterForwardVector();
	//获取Actor右向量
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCharacterRightVector();
	//获取Actor向上前向量
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCharacterUpVector();
	
/************************************Tick相关***************************************************/
private:
	//当前角色的Yaw
	float CurrentlyYaw;

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
