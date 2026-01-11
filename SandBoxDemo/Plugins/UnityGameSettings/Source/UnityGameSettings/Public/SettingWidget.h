#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxKey.h"
#include "GameFramework/GameUserSettings.h"
#include "GenericPlatform/GenericWindow.h" // 包含 EWindowMode
#include "Components/CanvasPanel.h"
#include "SettingWidget.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum ESettingPanel:uint8
{
	SeetingPanel_ViewPort = 0 UMETA(DisplayName = "画面"),
	SettingPanel_Audio=1 UMETA(Display="音频"),
	
};


UCLASS()
class UNITYGAMESETTINGS_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
/****************        面板        *********************/

	//主面板
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UCanvasPanel> MainPanel;

	//设置面板
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> SettingsPanel;

	//选择面板
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> SelectsPanel;

/*********************************************************/


/***************        选项面板       *******************/

	//设置按钮
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Settings_Btn;

	//返回游戏
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ReturnGame_Btn;

	//退出按钮
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Quit_Btn;

/*********************************************************/


/***************        设置面板       *******************/

	//面板切换器
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> SettingSwitcher;
	
	//视口切换按钮
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<UButton> ViewPort_Btn;

	//音频切换按钮
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<UButton> Audio_Btn;

	//介绍
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> IntroduceText;
	//取消
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> SettingsCancel_Btn;
	//应用
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> SettingsApply_Btn;

/*********************************************************/


/**************        视口设置       ********************/

	//窗口选择框
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxKey> WindowComboBox;
	//窗口组
	TMap<uint8,TEnumAsByte<EWindowMode::Type>>WindowMap;
	//当前窗口索引
	uint8 CurWindowIndex;

	//分辨率选择框
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UComboBoxKey> ResolutionComboBox;
	//分辨率组
	TMap<uint8, FIntPoint>ResolutionMap;
	//当前的分辨率
	uint8 CurResolutionIndex;

/*********************************************************/

protected:

	//当前的设置面板
	TEnumAsByte<ESettingPanel> CurSettingPanel;

protected:
/**************        绑定按钮       ********************/
	UFUNCTION()
	void OnAudioBtnClicked_Handler();
	UFUNCTION()
	void OnViewPortClicked_Handler();
	UFUNCTION()
	void OnSettingsCancelBtnClicked_Handler();
	void CloseSettingPanel();
	void OpenSettingPanel();
	//游戏设置按钮绑定事件
	UFUNCTION()
	void OnSettingsBtnClicked_Handler();
	//退出游戏绑定事件
	UFUNCTION()
	void OnQuitBtnClicked_Handler();
	//应用当前设置绑定事件
	UFUNCTION()
	void OnSettingsApplyBtnClicked_Handler();
	//返回游戏绑定事件
	UFUNCTION()
	void OnReturnGameClicked_Handler();
/*********************************************************/
	UFUNCTION()
	void InitViewPortSetting();
	
	//关闭所有面板
	void Close();
	UFUNCTION(BlueprintCallable)
	//开启
	void Open();

/**************        视口设置       ********************/
	//初始化分辨率
	UFUNCTION()
	void InitResolution();
	//初始化窗口
	UFUNCTION()
	void InitWindow();


public:
	//切换当前设置面板
	UFUNCTION()
	void SwitchSettingPanel(ESettingPanel NewSettingPanel);

};
