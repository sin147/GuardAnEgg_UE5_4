// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SandBoxActor_Base.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TimeLineComponent.h"
#include "Components/WidgetComponent.h"
#include "SandBoxPeople.generated.h"

class USphereComponent;
class UUI_SandBoxPeopleHP;
class UWidgetComponent;
class ASandBox_Prop;
/**
 * 
 */



 //阵营枚举
UENUM(BlueprintType)
enum class EPeopleCamp : uint8
{
	//无
	PeopleCamp_None UMETA(DisplayName = "未设置"),
	//红方
	PeopleCamp_Red UMETA(DisplayName = "红方"),
	//蓝方
	PeopleCamp_Blue UMETA(DisplayName = "蓝方"),
	//中立
	PeopleCamp_Neutral UMETA(DisplayName = "中立"),
};

 //人物状态枚举
UENUM(BlueprintType)
enum class EPeopleState : uint8
{
	//待机
	Idle UMETA(DisplayName = "待机"),
	//攻击
	Attack UMETA(DisplayName = "攻击"),
	//移动
	Move UMETA(DisplayName = "移动"),
	//死亡
	Dead UMETA(DisplayName = "死亡"),
	//受击
	BeHit UMETA(DisplayName = "受击"),
};

UCLASS()
class SANDBOXDEMO_API ASandBoxPeople : public ASandBoxActor_Base
{
	GENERATED_BODY()

public:
	ASandBoxPeople();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime)override;
protected:
	//骨骼网格组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "People")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	//默认碰撞检测组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "People")
	TObjectPtr<USphereComponent> CollisionDetectionComponent;
	//血条控件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "People")
	TObjectPtr<UWidgetComponent> HPBarWidget;
	//攻击范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|Attack")
	float AttackDetectionRadius;
	//索敌范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|Attack")
	float EnemyDetectionRadius;
	//防御力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|Attack")
	float DefensePower=5;
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|Attack")
	float AttackPower;
	//敌军阵营
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|Attack")
	TArray<EPeopleCamp> EnemyTypes;
	//友军阵营
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|Attack")
	TArray<EPeopleCamp> AllyTypes;
	//当前移动锁定的敌人
	UPROPERTY(EditAnywhere, Category = "People|Attack")
	TObjectPtr<ASandBoxPeople> CurrentlyLockedEnemy;
	//最终目标的敌人
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "People|Attack")
	TObjectPtr<ASandBoxPeople> EnemyKing;
	//当前攻击锁定的敌人
	UPROPERTY(EditAnywhere, Category = "People|Attack")
	TObjectPtr<ASandBoxPeople> CurrentlyAttackingEnemy;
	//当前人物阵营
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|Attack")
	EPeopleCamp CurrentCamp;
	//当前人物状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People")
	EPeopleState CurrentState;
	//人物之前的状态
	EPeopleState PreviousState;
	//血量	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|HP")
	float CurrentlyHP;
	//最大血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|HP")
	float MaxHP;
	//移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|Move")
	float MoveSpeed;
	//转向速度(°/s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "People|Move")
	float RotateSpeed;
	//动画组合
	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = "People|Animation")
	TMap<EPeopleState, TObjectPtr<UAnimSequence>> AnimMontages;
	//移动时间轴
	UPROPERTY()
 	TObjectPtr<UTimelineComponent> MoveTimeLine;
	//受击时间轴
	UPROPERTY()
	TObjectPtr<UTimelineComponent> BeHitTimeLine;
	//移动曲线
	UPROPERTY(EditAnywhere, Category = "People|Move")
	UCurveFloat* MoveCurve;
	//移动的开始位置
	FVector BeginMoveLocation;
	//移动的目标位置
	FVector EndMoveLocation;
	//转向时间轴
	UPROPERTY(EditAnywhere, Category = "People|Move")
	TObjectPtr<UTimelineComponent> RotateTimeLine;
	//转向曲线
	UPROPERTY(EditAnywhere, Category = "People|Move")
	UCurveFloat* RotateCurve;
	//转向开始位置
	FRotator BeginRotation;
	//转向结束位置
	FRotator EndRotation;
	//伤害预存
	float Damage;
	
protected:
	//转向
	void RotateToVector(FVector TargetVector);
	//当成功探测到敌人时调用
	UFUNCTION()
	virtual void OnEnemyDetected(ASandBoxPeople* DetectedPeople);
	//当成功探测到友军时调用
	UFUNCTION()
	virtual void OnAllyDetected(ASandBoxPeople* DetectedPeople);
	//当成功探测到可攻击目标时调用
	UFUNCTION()
	virtual void OnAttackDetected(ASandBoxPeople* DetectedPeople);
	//当移动时间轴开始更新时
	UFUNCTION()
	void OnMoveTimeLineUpdate(float Value);
	//当移动时间轴结束时
	UFUNCTION()
	void OnMoveTimeLineEnd();
	//当旋转时间轴开始更新时
	UFUNCTION()
	void OnRotateTimeLineUpdate(float Value);
	//当旋转时间轴结束时
	UFUNCTION()
	void OnRotateTimeLineEnd();
	//检测敌人
	UFUNCTION()
	bool EnemyDetection();
	//启用敌人检测
	bool EnableEnemyDetection=true;
	//攻击检测
	UFUNCTION()
	bool AttackDetection();
	//启用攻击检测
	bool EnableAttackDetection= false;
	//移动检测更新定时器
	FTimerHandle MoveUpdateTimer;
	//闪红定时器
	FTimerHandle BeRedTimer;
	//回复定时器
	FTimerHandle BeGreenTimer;
public:
	//修改护甲
	void SetDefensePower(float InDefensePower )
	{
		DefensePower = InDefensePower;
	}
	//获取护甲
	float GetDefensePower()
	{
		return DefensePower;
	}

	//近战攻击
	virtual void AttackEnemy();
	//回复效果逻辑
	UFUNCTION()
	void ApplyRestoreHP(float InRestoreValue);
	//受伤效果逻辑
	UFUNCTION()
	void OnBeHit();
	//手动判断攻击范围内是否还有敌人
	bool ManualDetectionAttackEnemy();
	//获取当前攻击对象
	ASandBoxPeople* GetCurrentlyAttackingEnemy();
	//获取当前阵营
	UFUNCTION(BlueprintCallable, Category = "People")
	EPeopleCamp GetCurrentCamp() const { return CurrentCamp; }
	//开始攻击
	UFUNCTION(BlueprintCallable, Category = "People")
	virtual void SwitchToAttackState();
	//切换为移动状态
	UFUNCTION(BlueprintCallable, Category = "People")
	virtual void SwitchToMoveState();
	//开始死亡
	UFUNCTION(BlueprintCallable, Category = "People")
	virtual void SwitchToDeadState();
	//待机
	UFUNCTION(BlueprintCallable, Category = "People")
	virtual void SwitchToIdleState();
	//受击
	UFUNCTION(BlueprintCallable, Category = "People")
	virtual void SwitchToBeHitState();
	//切换到前一个状态
	UFUNCTION(BlueprintCallable, Category = "People")
	void SwithToPreviousState();
	//获取当前状态
	UFUNCTION(BlueprintCallable, Category = "People")
	EPeopleState GetCurrentState() const { return CurrentState; }
	//存储伤害
	float StoreDamage(float InDamage);
	//人物向目标移动
	void MoveToEnemy();
	//转向到底敌人
	void RotateToEnemy();
	//停止移动
	void StopMove();
	//结束攻击
	void StopAttack();
	//受伤
	void ApplyDamage();
	//设置EnemyKing
	void SetEnemyKing(ASandBoxPeople*InEnemyKing);
	//判断是否为友军
	bool IsFriend(ASandBoxPeople*InPeople);
	//判断是否为敌军
	bool IsEnemy(ASandBoxPeople* InPeople);
	//获取当前血量
	float GetCurrentlyHP()
	{
		return CurrentlyHP;
	}
	//获取最大血量
	float GetMaxHP()
	{
		return MaxHP;
	}
};
static bool IsValidPeople(const ASandBoxPeople* TestPeople)
{
	return IsValid(TestPeople) && TestPeople->GetCurrentState() != EPeopleState::Dead;
}