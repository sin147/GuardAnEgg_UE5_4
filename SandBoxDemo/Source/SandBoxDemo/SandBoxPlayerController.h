// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "SandBoxPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASandBoxPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//转向加速度
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
	FRotator RotatorAcceleration=FRotator(30,60,0);

	//最大转向速率
	UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
	FRotator MaxRotationRate;
	virtual void UpdateRotation(float DeltaTime) override;
	FRotator GetRotationRate()
	{
		return CurrentlyRotationRate;
	}

protected:
	//当前转向速率
	//UPROPERTY(Replicated)
	FRotator CurrentlyRotationRate;
	FRotator Client_CurrentlyRotationRate;
	//同步转向速率到服务器
	UFUNCTION(Server,Reliable)
	void Server_SyncRotationRate(FRotator InRotation);
	//同步到所有客户端
	UFUNCTION(NetMulticast,Reliable)
	void NetMulticast_SyncRotationRate(FRotator InRotation);

	//计算当前转向速度
	UFUNCTION(BlueprintCallable)
	void CalculateDeltaRotation(FRotator& InRotationInput, float DeltaTime);
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
