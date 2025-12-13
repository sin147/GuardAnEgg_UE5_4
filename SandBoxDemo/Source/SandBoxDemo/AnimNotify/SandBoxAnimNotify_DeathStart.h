// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify/SandBoxAnimNotify_Base.h"
#include "SandBoxAnimNotify_DeathStart.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API USandBoxAnimNotify_DeathStart : public USandBoxAnimNotify_Base
{
	GENERATED_BODY()
protected:
	virtual void SandBoxNotify(ASandBoxPeople* SandBoxPeople) override;
};
