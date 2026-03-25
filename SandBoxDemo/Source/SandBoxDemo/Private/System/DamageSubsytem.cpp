// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DamageSubsytem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NetworkProxyActor/DamageProxyActor.h"

void UDamageSubsytem::TakeDamage(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{

	if (!IsValid(DamageProxy)) { return; }
	ENetMode NetMode = GetWorld()->GetNetMode();
	UKismetSystemLibrary::PrintString(GetWorld(),"TakeDamage");
	switch (NetMode)
	{
	case NM_Standalone:
		Multicast_TakeDamageImp(DamagedActor, DamageAmount, DamageCauser);
		break;
	case NM_DedicatedServer:
		Server_TakeDamageImp(DamagedActor, DamageAmount, DamageCauser);
		break;
	case NM_ListenServer:
		DamageProxy->Multicast_TakeDamage(DamagedActor, DamageAmount, DamageCauser);
		break;
	case NM_Client:
		//DamageProxy->Server_TakeDamage(DamagedActor, DamageAmount, DamageCauser);
		break;
	case NM_MAX:
		break;
	default:
		break;
	}
}

void UDamageSubsytem::SetDamageProxy(ADamageProxyActor* Proxy)
{
    if(GetWorld()->GetFirstPlayerController() == Proxy->Owner)
	{
		DamageProxy = Proxy;
	}
}

void UDamageSubsytem::Server_TakeDamageImp(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	if (!IsValid(DamageProxy)) { return; }

	if (DamagedActor)
	{

		if (Cast<IDamage>(DamagedActor) != nullptr)
		{
			//Cast<IDamage>(DamagedActor)->ApplyDamage(DamageAmount, DamageCauser);
			DamageProxy->Multicast_TakeDamage(DamagedActor, DamageAmount, DamageCauser);
		}
	}
}

void UDamageSubsytem::Multicast_TakeDamageImp(AActor* DamagedActor, float DamageAmount, AActor* DamageCauser)
{
	if (DamagedActor)
	{
		if (Cast<IDamage>(DamagedActor) != nullptr)
		{
			Cast<IDamage>(DamagedActor)->ApplyDamage(DamageAmount, DamageCauser);
		}
	}
}
