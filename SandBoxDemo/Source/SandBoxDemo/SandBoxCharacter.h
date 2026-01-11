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
#include "InputAction.h"
#include "SandBoxCharacter.generated.h"
class ASandBox_Prop;
class UUI_CharacterInfo;
UENUM(BlueprintType)
 enum class EAttackType : uint8
{
	AT_None,
	AT_CloseAttack,
	AT_FarAttack
};

USTRUCT(BlueprintType)
struct FCharacterInfo
{

	//血量
	float HP = 100;

	//移动速度
	float MaxSpeed = 800;

};



UCLASS()
class SANDBOXDEMO_API ASandBoxCharacter : public ACharacter
{
	
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASandBoxCharacter();
protected:
	//角色绑定的King
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	ASandBoxPeople_King* BindKing;
	//角色视口
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<UUI_CharacterInfo> CharacterInfoClass;
	//敌人类型
	UPROPERTY(EditAnywhere, Category = "Character")
	TArray<EPeopleCamp> EnemyTypes;
	//友军类型
	UPROPERTY(EditAnywhere, Category = "Character")
	TArray<EPeopleCamp> AllyTypes;

	//血量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	float MaxHP;
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float CurrentlyHP;
	//相机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;
	//相机弹簧臂
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;
	//输入上下文
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;
	//移动输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	//攻击类型
	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	EAttackType CurrentlyAttackType= EAttackType::AT_None;

	//近战攻击输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CloseAttackAction;
	UFUNCTION()
	void SwitchCloseAttack();
	//近战攻击
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void CloseAttack();
	//远程攻击输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FarAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack|FarAttack")
	TSubclassOf<ASandBox_Prop>FarAttackPropClass;
	//切换为远程攻击状态
	UFUNCTION()
	void SwitchFarAttack();
	//远程攻击
	UFUNCTION(BlueprintCallable, Category = "Attack|FarAttack")
	void FarAttack();
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	//查看输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	//移动锁定初始转向
	FRotator BlockLookRotation;
	//角色转向时间轴
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTimelineComponent* CharacterRotationTimeline;
	//更新转向
	UPROPERTY(EditAnywhere, Category = "Character|Move")
	UCurveFloat* RotationCurve;
	UFUNCTION()
	void UpdateRotation(float value);
	UFUNCTION()
	void UpdateRotationFinish();
	void UpdateCharacterRotation();
	//初始转向
	FRotator BeginRotation;
	//目标转向
	FRotator TargetRotation;
	//闪红定时器
	FTimerHandle BeRedTimer;
	//士兵1生成定时器
	FTimerHandle Solder1SpawnTimer;
	//士兵2生成定时器
	FTimerHandle Solder2SpawnTimer;
	//士兵3生成定时器
	FTimerHandle Solder3SpawnTimer;
	//士兵4生成定时器
	FTimerHandle Solder4SpawnTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	//获取绑定的King
	ASandBoxPeople_King* GetKing()
	{
		return BindKing;
	}
	//设置绑定的King
	void SetKing(ASandBoxPeople_King* InBindKing)
	{
		 BindKing=InBindKing;
	}
	//获取最大血量
	float GetMaxHP()
	{
		return MaxHP;
	}
	//获取当前血量
	float GetCurrentlyHP()
	{
		return CurrentlyHP;
	}

	void BeHit(float Damage);
	//玩家阵营
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	EPeopleCamp CurrentCamp;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
