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
	IT_Active  UMETA(DisplayName = "Active"),
	IT_Passive UMETA(DisplayName = "Passive"),
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
	TArray<TObjectPtr<ACharacter>> InteractiveCharacters;
	//交互已经进行的时间
	float InteractTime;

	//交互准备完成时间
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess=true))
	float PreInteractDuration=0;
	//交互完成时间
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = true))
	float InteractDuration=0;
	//交互结束完成时间
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = true))
	float InteractOverDuration=0;
	//交互中断完成时间
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive", meta = (AllowPrivateAccess = true))
	float InteractBreakDuration=0;
	//准备交互
	void PreInteract(float DeltaTime);
	//交互中
	void Interactting(float DeltaTime);
	//交互完成
	void InteractOver(float DeltaTime);
	//交互中断
	void InteractBreak(float DeltaTime);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//交互触发框
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
	class USphereComponent* TriggerBox;
	//交互触发类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive")
	TEnumAsByte<EInteractiveType> InteractiveType=EInteractiveType::IT_Active;
	//交互触发事件
	UFUNCTION()
	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//交互结束事件
	UFUNCTION()
	void OnTriggerBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//准备交互实现
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void PreInteractImp(float DeltaTime);
	//交互实现
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void InteracttingImp(float DeltaTime);
	//交互完成实现
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void InteractOverImp(float DeltaTime);
	//交互中断实现
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void InteractBreakImp(float DeltaTime);
	//获取当前交互的玩家组
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interact")
	TArray<ACharacter*> GetInteractiveCharacters();

public:	
	//获取当前交互类型
	UFUNCTION(BlueprintCallable, Category = "Interactive")
	EInteractiveType GetInteractiveType() const { return InteractiveType; }
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//交互接口实现
	void Interact(ACharacter* InCharacter);
	//是否可以交互
	virtual bool CanInteract(ACharacter* InCharacter) override;
};
