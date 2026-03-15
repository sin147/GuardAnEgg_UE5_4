// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interface/Damage.h"
#include "DamageSubsytem.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UDamageSubsytem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//造成伤害
	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser);

};
