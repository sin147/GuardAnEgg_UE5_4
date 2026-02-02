// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "SandBoxPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASandBoxPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//开始转向加速
	bool bStartYawRotator = false;
	bool bStartPitchRotator = false;
	//转向加速度
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
	FRotator RotatorAcceleration=FRotator(30,60,0);
	//当前转向速率
	FRotator CurrentlyRotationRate;
	//最大转向速率
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
	FRotator MaxRotationRate;
	virtual void UpdateRotation(float DeltaTime) override;
protected:
	//计算当前转向速度
	UFUNCTION(BlueprintCallable)
	void CalculateDeltaRotation(FRotator& InRotationInput, float DeltaTime);
};
