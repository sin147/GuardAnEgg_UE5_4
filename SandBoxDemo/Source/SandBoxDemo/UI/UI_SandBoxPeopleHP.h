// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_SandBoxPeopleHP.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UUI_SandBoxPeopleHP : public UUserWidget
{
	GENERATED_BODY()
protected:
	//定时器
	FTimerHandle ShowHPTimerHandle;

	//进度条控件
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPProgressBar;
	//血量文本
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HPText;
public:
	UFUNCTION(BlueprintCallable, Category = "PeopleHP")
	void SetHP(float InMaxHP,float InCurrentlyHP);

};
