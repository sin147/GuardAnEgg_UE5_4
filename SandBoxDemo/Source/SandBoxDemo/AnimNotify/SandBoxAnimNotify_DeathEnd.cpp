// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/SandBoxAnimNotify_DeathEnd.h"

void USandBoxAnimNotify_DeathEnd::SandBoxNotify(ASandBoxPeople* SandBoxPeople)
{
	Super::SandBoxNotify(SandBoxPeople);
	SandBoxPeople->Destroy();
}
