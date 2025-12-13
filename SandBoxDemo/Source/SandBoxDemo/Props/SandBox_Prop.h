// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SandBoxActor_Base.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "../SandBoxPeople.h"
#include "../SandBoxCharacter.h"
#include "SandBox_Prop.generated.h"


/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASandBox_Prop : public ASandBoxActor_Base
{
	GENERATED_BODY()
public:
	ASandBox_Prop();
	virtual void BeginPlay() override;
protected:
	//伤害量
	UPROPERTY(EditAnywhere, Category = "Prop")
	float DamageValue = 0;
	//影响范围缩放
	UPROPERTY(EditAnywhere, Category = "Prop")
	float ApplyScale = 2;

	//回复量
	UPROPERTY(EditAnywhere, Category = "Prop")
	float RestoreValue = 0;
	//碰撞检测组件
	UPROPERTY(VisibleAnywhere, Category = "Prop")
	USphereComponent* CollisionComponent;
	//移动曲线
	UPROPERTY(EditAnywhere, Category = "Move");
	UCurveFloat* MoveCurve;
	//爆点
	UPROPERTY(EditAnywhere, Category = "Prop")
	TObjectPtr<UNiagaraSystem> ShotPoint;
	//道具移动时间轴
	UPROPERTY()
	TObjectPtr<UTimelineComponent> MoveTimeLine;
	//移动起始位置
	FVector BeginLocation;
	//移动结束位置
	FVector EndLocation;
	//移动速度
	UPROPERTY(EditAnywhere, Category = "Move")
	float MoveSpeed=2000;
	//敌军类型
	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<EPeopleCamp> EnemyTypes;
	//友军类型
	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<EPeopleCamp> AllyTypes;

	//当移动更新时
	UFUNCTION()
	void OnMoveUpdate(float value);
	

	//当移动结束时
	UFUNCTION()
	void OnMoveEnd();
public:
	//应用到Peoples
	void ApplyToPeoples(TArray<TObjectPtr<ASandBoxPeople>> Enemys, TArray<TObjectPtr<ASandBoxPeople>> Allys);
	//移动到目标点
	UFUNCTION(BlueprintCallable)
	void MoveToLocation(FVector Location);
	//设置碰撞忽略
	void InitialPropToApply(TArray<EPeopleCamp> InEnemyTypes,TArray<EPeopleCamp> InAllyTypes, TArray<TObjectPtr<AActor>> InIgnoreActors);

protected:
	//当开始碰撞
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

};
