// Fill out your copyright notice in the Description page of Project Settings.


#include "System/CharacterSubsystem.h"

void UCharacterSubsystem::Server_SetCharacterStateByEnum(ECharacterAttribute InAttribute, float InNewValue, ECAVType InValueType)
{
}

void UCharacterSubsystem::Multicast_SetCharacterStateByEnum(ECharacterAttribute InAttribute, float InNewValue, ECAVType InValueType)
{
}

void UCharacterSubsystem::SetCharacterStateByEnum(ECharacterAttribute InAttribute, float InNewValue, ECAVType InValueType)
{
	/*if (!IsValid(CharacterProxy)) { return; }
	CharacterProxy->Server_SetCharacterStateByEnum(InAttribute, InNewValue, InValueType);*/
}
