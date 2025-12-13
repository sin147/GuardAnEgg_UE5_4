// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SandBoxPeopleHP.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUI_SandBoxPeopleHP::SetHP(float InMaxHP, float InCurrentlyHP)
{
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(InCurrentlyHP / InMaxHP);
	}
	if (HPText)
	{
		FString HPString = FString::Printf(TEXT("%.0f / %.0f"), InCurrentlyHP, InMaxHP);
		HPText->SetText(FText::FromString(HPString));
	}
	SetVisibility(ESlateVisibility::Visible);
	TWeakObjectPtr<UUI_SandBoxPeopleHP> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(
		ShowHPTimerHandle,
		[WeakThis]() // 捕获弱引用
		{
			if (WeakThis.IsValid())
			{
				WeakThis->SetVisibility(ESlateVisibility::Hidden);
			}
			
		},
		3.0f, // 检测间隔
		false  // 循环
	);
}
