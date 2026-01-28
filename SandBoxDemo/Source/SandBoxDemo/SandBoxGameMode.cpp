// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxGameMode.h"
#include "SandBoxPlayerController.h"
#include "SandBoxCharacter.h"
#include "SandBoxHUD.h"

ASandBoxGameMode::ASandBoxGameMode()
{
	DefaultPawnClass = ASandBoxCharacter::StaticClass();
	HUDClass = ASandBoxHUD::StaticClass();
	PlayerControllerClass = ASandBoxPlayerController::StaticClass();
}

void ASandBoxGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
