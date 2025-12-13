// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/SandBoxAnimNotify_IdleStart.h"

void USandBoxAnimNotify_IdleStart::SandBoxNotify(ASandBoxPeople* SandBoxPeople)
{
	Super::SandBoxNotify(SandBoxPeople);
	SandBoxPeople->StopMove();
}
