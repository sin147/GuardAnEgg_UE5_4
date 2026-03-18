// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DamageSubsytem.h"
#include "NetworkProxyActor/DamageProxyActor.h"

void UDamageSubsytem::TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	if (!IsValid(DamageProxy)) { return; }
	ENetMode NetMode = GetWorld()->GetNetMode();
	if (NetMode == NM_ListenServer || NetMode == NM_DedicatedServer)
	{
		Server_TakeDamage(DamagedActor, DamageAmount, DamageCauser);

	}
	else
	{
		DamageProxy->Server_TakeDamage(DamagedActor, DamageAmount, DamageCauser);
	}
}

void UDamageSubsytem::Server_TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	if (!IsValid(DamageProxy)) { return; }

	if (DamagedActor)
	{

		if (Cast<IDamage>(DamagedActor) != nullptr)
		{
			Cast<IDamage>(DamagedActor)->ApplyDamage(DamageAmount, DamageCauser);
			DamageProxy->Multicast_TakeDamage(DamagedActor, DamageAmount, DamageCauser);
		}
	}
}

void UDamageSubsytem::Multicast_TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	if (DamagedActor)
	{
		if (Cast<IDamage>(DamagedActor) != nullptr)
		{
			Cast<IDamage>(DamagedActor)->ApplyDamage(DamageAmount, DamageCauser);
		}
	}
}
