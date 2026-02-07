// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "EnhancedInputModule.h"
#include "EnhancedPlayerInput.h"
#include "Double.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "Double"))
class UDouble : public UInputTrigger
{
	GENERATED_BODY()
public:
    // 最大允许双击间隔（秒）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings")
    float MaxInterval = 1.f;

protected:
    // 第一次完成按下时间
    float FirstTapTime = -1.f;
    // 第二次完成按下时间
    float SecondTapTime = -1.f;
    //上一次是否为按下
    bool bLastIsDown = false;

    //准备触发
    bool bReadyTrigger;

    virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override
    {
        bool bIsDown = ModifiedValue.Get<bool>();
        ETriggerState ReState=ETriggerState::None;
        float CurrentTime = PlayerInput->GetWorld()->GetTimeSeconds();
        if (bIsDown or bReadyTrigger)
        {
            ReState= ETriggerState::Ongoing;
        }
        if (!bReadyTrigger and bLastIsDown and bLastIsDown != bIsDown)
        {
            FirstTapTime = CurrentTime;
            bReadyTrigger = true;
        }
        else if (bReadyTrigger and bLastIsDown and bLastIsDown != bIsDown)
        {
            SecondTapTime = CurrentTime;
            if (SecondTapTime - FirstTapTime <= MaxInterval)
            {
                ReState = ETriggerState::Triggered;
            }
            else
            {
                ReState = ETriggerState::None;
            }
            bReadyTrigger = false;
        }
        if (CurrentTime - FirstTapTime > MaxInterval)
        {
            bReadyTrigger = false;
        }

        bLastIsDown = bIsDown;
        return ReState;
    }

    virtual ETriggerType GetTriggerType_Implementation() const override
    {
        return ETriggerType::Explicit;
    }
};
