// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxPlayerController.h"
#include "SandBoxCharacter.h"

void ASandBoxPlayerController::UpdateRotation(float DeltaTime)
{
	// Calculate Delta to be applied on ViewRotation
	CalculateDeltaRotation(RotationInput, DeltaTime);
	Super::UpdateRotation(DeltaTime);
}

void ASandBoxPlayerController::CalculateDeltaRotation(FRotator& InRotationInput, float DeltaTime)
{

		if (InRotationInput.Yaw != 0)
		{
			CurrentlyRotationRate.Yaw = FMath::Clamp(CurrentlyRotationRate.Yaw + RotatorAcceleration.Yaw * DeltaTime * InRotationInput.Yaw, -MaxRotationRate.Yaw, MaxRotationRate.Yaw);
		}
		else if(CurrentlyRotationRate.Yaw != 0)
		{

			CurrentlyRotationRate.Yaw = CurrentlyRotationRate.Yaw > 0 ? CurrentlyRotationRate.Yaw - 1 : CurrentlyRotationRate.Yaw + 1;
			CurrentlyRotationRate.Yaw = FMath::IsNearlyEqual(CurrentlyRotationRate.Yaw, 0, 10) ? 0 : CurrentlyRotationRate.Yaw;
		}

		if (InRotationInput.Pitch != 0)
		{
			CurrentlyRotationRate.Pitch = FMath::Clamp(CurrentlyRotationRate.Pitch + RotatorAcceleration.Pitch * DeltaTime * InRotationInput.Pitch, -MaxRotationRate.Pitch, MaxRotationRate.Pitch);
		}
		else if (!FMath::IsNearlyEqual( GetControlRotation().Pitch,0,10))
		{
			
			CurrentlyRotationRate.Pitch = FMath::IsNearlyEqual(GetControlRotation().Pitch, 90, 90)?-MaxRotationRate.Pitch: MaxRotationRate.Pitch;
			UE_LOG(LogTemp, Log, TEXT("%lf"), CurrentlyRotationRate.Pitch);
		}
		else
		{
			CurrentlyRotationRate.Pitch = 0;
		}

		if (InRotationInput.Roll != 0)
		{
			CurrentlyRotationRate.Roll = FMath::Clamp(CurrentlyRotationRate.Roll + RotatorAcceleration.Roll * DeltaTime * InRotationInput.Roll, -MaxRotationRate.Roll, MaxRotationRate.Roll);
		}
		else if (CurrentlyRotationRate.Roll != 0)
		{
			CurrentlyRotationRate.Roll = CurrentlyRotationRate.Roll > 0 ? CurrentlyRotationRate.Roll - 1 : CurrentlyRotationRate.Roll + 1;
			CurrentlyRotationRate.Roll = FMath::IsNearlyEqual(CurrentlyRotationRate.Roll, 0, 10) ? 0 : CurrentlyRotationRate.Roll;
		}
	InRotationInput=CurrentlyRotationRate * DeltaTime;
}