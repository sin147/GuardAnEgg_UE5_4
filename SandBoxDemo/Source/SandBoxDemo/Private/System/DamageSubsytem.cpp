// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DamageSubsytem.h"
#include "NetworkProxyActor/DamageProxyActor.h"

void UDamageSubsytem::TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	DamageProxy->Server_TakeDamage(DamagedActor, DamageAmount, DamageCauser);
}

void UDamageSubsytem::Server_TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
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
