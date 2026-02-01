// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SandBoxMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API USandBoxMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	//开始转向加速
	bool bStartYawRotator=false;
	bool bStartPitchRotator = false;
	//转向加速度
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
	FRotator RotatorAcceleration;
	//最大转向速率
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
	FRotator MaxRotationRate;
protected:
	//计算当前转向速度
	UFUNCTION(BlueprintCallable)
	void CalculateRotationRate(float DeltaTime);
	virtual void PhysicsRotation(float DeltaTime) override;
};
