// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interface/Damage.h"
#include "DamageSubsytem.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API UDamageSubsytem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	friend class ADamageProxyActor;
private:
	//伤害代理
	ADamageProxyActor* DamageProxy;

public:
	//造成伤害
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser);
protected:
	//设置伤害代理
	void SetDamageProxy(ADamageProxyActor* Proxy) { DamageProxy = Proxy; }
	//服务器处理伤害
	void Server_TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser);
	//多播处理伤害
	void Multicast_TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser);

};
