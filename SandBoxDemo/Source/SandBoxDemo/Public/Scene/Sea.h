// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Scene/SceneBase.h"
#include "Sea.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASea : public ASceneBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
private:
	TObjectPtr<APhysicsVolume> WaterVolume;

	//生成PhysicsVolume
	void InitalPhysicsVolume();
};
