// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Damage.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SANDBOXDEMO_API IDamage
{
	GENERATED_BODY()
	friend class UDamageSubsytem;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	//应用伤害
	virtual void ApplyDamage(float DamageAmount,AActor* DamageCauser)=0;
};
