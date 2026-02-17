// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxPlayerController.h"
#include "Net/UnrealNetwork.h"
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

			CurrentlyRotationRate.Yaw = CurrentlyRotationRate.Yaw > 0 ? CurrentlyRotationRate.Yaw - 3 : CurrentlyRotationRate.Yaw + 3;
			CurrentlyRotationRate.Yaw = FMath::IsNearlyEqual(CurrentlyRotationRate.Yaw, 0, 1) ? 0 : CurrentlyRotationRate.Yaw;
		}

		if (InRotationInput.Pitch != 0)
		{
			CurrentlyRotationRate.Pitch = FMath::Clamp(CurrentlyRotationRate.Pitch + RotatorAcceleration.Pitch * DeltaTime * InRotationInput.Pitch, -MaxRotationRate.Pitch, MaxRotationRate.Pitch);
		}
		else if (!FMath::IsNearlyEqual( GetControlRotation().Pitch,0,1))
		{
			
			CurrentlyRotationRate.Pitch = FMath::IsNearlyEqual(GetControlRotation().Pitch, 90, 90)?-MaxRotationRate.Pitch: MaxRotationRate.Pitch;
			InRotationInput.Pitch = CurrentlyRotationRate.Pitch * DeltaTime;
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
			CurrentlyRotationRate.Roll = FMath::IsNearlyEqual(CurrentlyRotationRate.Roll, 0, 1) ? 0 : CurrentlyRotationRate.Roll;

		}
		if (InRotationInput.Yaw || InRotationInput.Pitch || InRotationInput.Roll)
		{
			InRotationInput = CurrentlyRotationRate * DeltaTime;
		}

}

void ASandBoxPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// These used to only replicate if PlayerCameraManager->GetViewTargetPawn() != GetPawn()
	// But, since they also don't update unless that condition is true, these values won't change, thus won't send
	// This is a little less efficient, but fits into the new condition system well, and shouldn't really add much overhead
	DOREPLIFETIME(ASandBoxPlayerController, CurrentlyRotationRate);
}
