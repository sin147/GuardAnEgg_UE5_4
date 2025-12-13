// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../SandBoxPeople.h"
#include "SandBoxAnimNotify_Base.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API USandBoxAnimNotify_Base : public UAnimNotify
{
	GENERATED_BODY()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
protected:
	virtual void SandBoxNotify(ASandBoxPeople* SandBoxPeople);
};
