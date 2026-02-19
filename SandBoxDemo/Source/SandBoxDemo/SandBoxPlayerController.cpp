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

void ASandBoxPlayerController::Server_SyncRotationRate_Implementation(FRotator InRotation)
{
	CurrentlyRotationRate = InRotation;
	NetMulticast_SyncRotationRate(CurrentlyRotationRate);
}


void ASandBoxPlayerController::NetMulticast_SyncRotationRate_Implementation(FRotator InRotation)
{
	CurrentlyRotationRate = InRotation;
}

void ASandBoxPlayerController::CalculateDeltaRotation(FRotator& InRotationInput, float DeltaTime)
{

		if (InRotationInput.Yaw != 0)
		{
			Client_CurrentlyRotationRate.Yaw = FMath::Clamp(Client_CurrentlyRotationRate.Yaw + RotatorAcceleration.Yaw * DeltaTime * InRotationInput.Yaw, -MaxRotationRate.Yaw, MaxRotationRate.Yaw);
		}
		else if(Client_CurrentlyRotationRate.Yaw != 0)
		{

			Client_CurrentlyRotationRate.Yaw = Client_CurrentlyRotationRate.Yaw > 0 ? Client_CurrentlyRotationRate.Yaw - 3 : Client_CurrentlyRotationRate.Yaw + 3;
			Client_CurrentlyRotationRate.Yaw = FMath::IsNearlyEqual(Client_CurrentlyRotationRate.Yaw, 0, 1) ? 0 : Client_CurrentlyRotationRate.Yaw;
		}

		if (InRotationInput.Pitch != 0)
		{
			Client_CurrentlyRotationRate.Pitch = FMath::Clamp(Client_CurrentlyRotationRate.Pitch + RotatorAcceleration.Pitch * DeltaTime * InRotationInput.Pitch, -MaxRotationRate.Pitch, MaxRotationRate.Pitch);
		}
		else if (!FMath::IsNearlyEqual( GetControlRotation().Pitch,0,1))
		{
			
			Client_CurrentlyRotationRate.Pitch = FMath::IsNearlyEqual(GetControlRotation().Pitch, 90, 90)?-MaxRotationRate.Pitch: MaxRotationRate.Pitch;
			InRotationInput.Pitch = Client_CurrentlyRotationRate.Pitch * DeltaTime;
		}
		else
		{
			Client_CurrentlyRotationRate.Pitch = 0;
		}

		if (InRotationInput.Roll != 0)
		{
			Client_CurrentlyRotationRate.Roll = FMath::Clamp(Client_CurrentlyRotationRate.Roll + RotatorAcceleration.Roll * DeltaTime * InRotationInput.Roll, -MaxRotationRate.Roll, MaxRotationRate.Roll);
		}
		else if (Client_CurrentlyRotationRate.Roll != 0)
		{
			Client_CurrentlyRotationRate.Roll = Client_CurrentlyRotationRate.Roll > 0 ? Client_CurrentlyRotationRate.Roll - 1 : Client_CurrentlyRotationRate.Roll + 1;
			Client_CurrentlyRotationRate.Roll = FMath::IsNearlyEqual(Client_CurrentlyRotationRate.Roll, 0, 1) ? 0 : Client_CurrentlyRotationRate.Roll;

		}
		if (InRotationInput.Yaw || InRotationInput.Pitch || InRotationInput.Roll)
		{
			InRotationInput = Client_CurrentlyRotationRate * DeltaTime;
		}
		if (IsLocalController())
		{
			Server_SyncRotationRate_Implementation(Client_CurrentlyRotationRate);
		}

}

void ASandBoxPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// These used to only replicate if PlayerCameraManager->GetViewTargetPawn() != GetPawn()
	// But, since they also don't update unless that condition is true, these values won't change, thus won't send
	// This is a little less efficient, but fits into the new condition system well, and shouldn't really add much overhead
	//DOREPLIFETIME(ASandBoxPlayerController, Client_CurrentlyRotationRate);
}
