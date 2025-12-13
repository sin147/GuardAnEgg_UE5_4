// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify/SandBoxAnimNotify_Base.h"
#include "SandBoxAnimNotify_Attacking.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API USandBoxAnimNotify_Attacking : public USandBoxAnimNotify_Base
{
	GENERATED_BODY()
protected:
	virtual void SandBoxNotify(ASandBoxPeople* SandBoxPeople) override;
	
};
