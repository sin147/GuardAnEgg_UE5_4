// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DamageSubsytem.h"

bool UDamageSubsytem::TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	if (DamagedActor)
	{
		
		if (Cast<IDamage>(DamagedActor)!=nullptr)
		{
			return Cast<IDamage>(DamagedActor)->ApplyDamage(DamageAmount, DamageCauser);
		}
	}
	return false;
}
