// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxGameMode.h"
#include "SandBoxPlayerController.h"
#include "SandBoxCharacter.h"
#include "NetworkProxyActor/NetworkProxyActor.h"
#include "System/MessageSystem.h"
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
	//给玩家添加网络代理
		//生成各个子系统的网络代理
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = NewPlayer;
	SpawnParameters.bNoFail = true;
	//GetWorld()->SpawnActor<AMessageProxyActor>(AMessageProxyActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	GetWorld()->SpawnActor<AInteractiveProxyActor>(AInteractiveProxyActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	GetWorld()->SpawnActor<AAttributeProxyActor>(AAttributeProxyActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	GetWorld()->SpawnActor<ACharacterProxyActor>(ACharacterProxyActor ::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	GetWorld()->SpawnActor<ADamageProxyActor>(ADamageProxyActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
}

void ASandBoxGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName,Options,ErrorMessage);


}
