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
private:
	bool IsBack;
public:
	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const override;
};
