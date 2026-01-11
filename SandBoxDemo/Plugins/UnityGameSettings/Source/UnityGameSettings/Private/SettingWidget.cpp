// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TWeakObjectPtr<USettingWidget> WeakThis;
	Close();
	
	//各设置页面切换绑定
	ViewPort_Btn->OnClicked.AddDynamic(this, &USettingWidget::OnViewPortClicked_Handler);
	Audio_Btn->OnClicked.AddDynamic(this, &USettingWidget::OnAudioBtnClicked_Handler);
	SettingsCancel_Btn->OnClicked.AddDynamic(this, &USettingWidget::OnSettingsCancelBtnClicked_Handler);
	SettingsApply_Btn->OnClicked.AddDynamic(this, &USettingWidget::OnSettingsApplyBtnClicked_Handler);
	Settings_Btn->OnClicked.AddDynamic(this, &USettingWidget::OnSettingsBtnClicked_Handler);
	Quit_Btn->OnClicked.AddDynamic(this, &USettingWidget::OnQuitBtnClicked_Handler);
	ReturnGame_Btn->OnClicked.AddDynamic(this, &USettingWidget::OnReturnGameClicked_Handler);
	IntroduceText->SetText(FText::FromString(L"视口设置"));
	InitViewPortSetting();
}

void USettingWidget::SwitchSettingPanel(ESettingPanel NewSettingPanel)
{
	SettingSwitcher->SetActiveWidgetIndex(NewSettingPanel);
}

void USettingWidget::OnAudioBtnClicked_Handler()
{
	this->CurSettingPanel = SettingPanel_Audio;
	SwitchSettingPanel(this->CurSettingPanel);
	IntroduceText->SetText(FText::FromString(L"音频设置"));
}

void USettingWidget::OnViewPortClicked_Handler()
{
	this->CurSettingPanel = SeetingPanel_ViewPort;
	SwitchSettingPanel(this->CurSettingPanel);
	IntroduceText->SetText(FText::FromString(L"视口设置"));
}


void USettingWidget::OnSettingsCancelBtnClicked_Handler()
{

	Close();

}

void USettingWidget::CloseSettingPanel()
{
	//关闭其余面板
	this->SettingsPanel->SetVisibility(ESlateVisibility::Collapsed);

	this->SelectsPanel->SetVisibility(ESlateVisibility::Visible);

}

void USettingWidget::OpenSettingPanel()
{
	//关闭其余面板
	this->SelectsPanel->SetVisibility(ESlateVisibility::Collapsed);


	this->SettingsPanel->SetVisibility(ESlateVisibility::Visible);

}

void USettingWidget::OnSettingsBtnClicked_Handler()
{
	OpenSettingPanel();
}

void USettingWidget::OnQuitBtnClicked_Handler()
{

	UKismetSystemLibrary::QuitGame(GetWorld(),GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,false);

}

void USettingWidget::OnSettingsApplyBtnClicked_Handler()
{
}

void USettingWidget::OnReturnGameClicked_Handler()
{
	Close();
}

void USettingWidget::InitViewPortSetting()
{
	InitResolution();
	InitWindow();
}

void USettingWidget::Close()
{
	//隐藏鼠标
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	//关闭控件
	this->SetVisibility(ESlateVisibility::Collapsed);
	
}

void USettingWidget::Open()
{
	//显示控件
	this->SetVisibility(ESlateVisibility::Visible);

	//关闭除选择面板的其余面板
	SettingsPanel->SetVisibility(ESlateVisibility::Collapsed);

	//仅打开选项面板
	SelectsPanel->SetVisibility(ESlateVisibility::Visible);

	//显示鼠标
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

}

void USettingWidget::InitResolution()
{	
	TArray<FIntPoint> Resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	int Index = 0;
	for (FIntPoint Point : Resolutions)
	{
		ResolutionMap.Add(Index, Point);
		FString OptionText = FString::Printf(TEXT("%dX%d"), Point.X, Point.Y);
		ResolutionComboBox->AddOption(*OptionText);

		Index++;
	}
	CurResolutionIndex = Index;
	ResolutionComboBox->SetSelectedOption(*FString::Printf(TEXT("%dX%d"), Resolutions[Resolutions.Num()-1].X, Resolutions[Resolutions.Num()-1].Y));
	
}

void USettingWidget::InitWindow()
{
	WindowMap.Add(0, EWindowMode::Type::Fullscreen);
	WindowComboBox->AddOption(L"全屏");

	WindowMap.Add(1, EWindowMode::Type::WindowedFullscreen);
	WindowComboBox->AddOption(L"无边框");

	WindowMap.Add(2, EWindowMode::Type::Windowed);
	WindowComboBox->AddOption(L"窗口化");

	WindowComboBox->SetSelectedOption(L"全屏");

	CurWindowIndex = 0;
}
