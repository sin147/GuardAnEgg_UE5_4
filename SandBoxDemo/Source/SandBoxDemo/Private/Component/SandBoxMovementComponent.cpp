// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SandBoxMovementComponent.h"
#include "GameFramework/Character.h"
void USandBoxMovementComponent::CalculateRotationRate(float DeltaTime)
{
	if (!RotationRate.Equals(MaxRotationRate)&& Acceleration.SizeSquared() >= UE_KINDA_SMALL_NUMBER)
	{
		RotationRate.Yaw = bStartYawRotator ?FMath::Clamp(RotationRate.Yaw + RotatorAcceleration.Yaw * DeltaTime, 0, MaxRotationRate.Yaw):0;
		RotationRate.Pitch = bStartPitchRotator? FMath::Clamp(RotationRate.Pitch + RotatorAcceleration.Pitch * DeltaTime, 0, MaxRotationRate.Pitch):0;
		//RotationRate.Roll = FMath::Clamp(RotationRate.Roll + RotatorAcceleration.Roll * DeltaTime, 0, MaxRotationRate.Roll);
	}
	else
	{
		RotationRate = FRotator::ZeroRotator;
		bStartYawRotator = false;
		bStartPitchRotator = false;
	}

}

void USandBoxMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (!(bOrientRotationToMovement || bUseControllerDesiredRotation))
	{
		return;
	}

	if (!HasValidData() || (!CharacterOwner->Controller && !bRunPhysicsWithNoController))
	{
		return;
	}
	//先计算旋转速度
	CalculateRotationRate(DeltaTime);
	Super::PhysicsRotation(DeltaTime);

}
