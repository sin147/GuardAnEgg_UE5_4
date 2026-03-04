// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interact.h"
#include "../State/StateMachineBase.h"
#include "InteractiveActor.generated.h"

UENUM(BlueprintType)
enum EInteractiveType : uint8
{
	IT_None UMETA(DisplayName = "None"),
	IT_Active  UMETA(DisplayName = "主动"),
	IT_Passive UMETA(DisplayName = "被动"),
};


UCLASS()
class SANDBOXDEMO_API AInteractiveActor : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();
private:
	//交互状态机
	TObjectPtr<UStateMachineBase> InteractiveStateMachine;
	//当前交互玩家
	TArray<ASandBoxCharacter> InteractiveCharacters;
	//交互已经进行的时间
	float InteractTime;

	//交互准备完成时间
	float PreInteractFinishTime=0;
	//交互完成时间
	float InteractFinishTime=0;
	//交互结束完成时间
	float InteractOverFinishTime=0;
	//交互中断完成时间
	float InteractBreakFinishTime=0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//交互触发框
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
	class USphereComponent* TriggerBox;
	//交互触发类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive")
	TEnumAsByte<EInteractiveType> InteractiveType=EInteractiveType::IT_None;
	//交互范围

	//交互触发事件
	UFUNCTION()
	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//交互结束事件
	UFUNCTION()
	void OnTriggerBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//准备交互
	void PreInteract(float DeltaTime);
	//交互中
	void Interactting(float DeltaTime);
	//交互完成
	void InteractOver(float DeltaTime);
	//交互中断
	void InteractBreak(float DeltaTime);
public:	
	//获取当前交互类型
	UFUNCTION(BlueprintCallable, Category = "Interactive")
	EInteractiveType GetInteractiveType() const { return InteractiveType; }
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//交互接口实现
	virtual void Interact(ACharacter* InCharacter) override;
	//是否可以交互
	virtual bool CanInteract(ACharacter* InCharacter) override;
	
	
};
