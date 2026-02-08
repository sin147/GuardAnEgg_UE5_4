// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SceneBase.generated.h"

UCLASS()
class SANDBOXDEMO_API ASceneBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASceneBase();
	/*************************环境空间*****************************/
private:
	//碰撞组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SceneBase", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;
protected:
	//当碰撞发生时调用
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//当碰撞结束时调用
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//获取范围
	FVector GetBoxExtent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
