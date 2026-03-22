// Fill out your copyright notice in the Description page of Project Settings.


#include "System/AttributeSubsystem.h"
#include "Interface/AttributeInterface.h"
#include "NetworkProxyActor/AttributeProxyActor.h"

void UAttributeSubsystem::SetAttributeProxy(AAttributeProxyActor* InProxyActor)
{
    if(GetWorld()->GetFirstPlayerController() == InProxyActor->Owner)
	{
		AttributeProxy = InProxyActor;
	}
}

void UAttributeSubsystem::Server_SetAttributeByEnum(AActor* InActor, EAttribute InAttribute, float InValue, ECAVType InAttributeValueType)
{
	if (!IsValid(AttributeProxy)) { return; }
	IAttributeInterface* AttributeInterface = Cast<IAttributeInterface>(InActor);
	if (AttributeInterface != nullptr)
	{
		AttributeInterface->SetAttributeByEnum(InAttribute, InValue, InAttributeValueType);
		AttributeProxy->Multicast_SetAttributeByEnum(InActor, InAttribute, InValue, InAttributeValueType);
	}
}

void UAttributeSubsystem::Multicast_SetAttributeByEnum(AActor* InActor, EAttribute InAttribute, float InValue, ECAVType InAttributeValueType)
{
	IAttributeInterface* AttributeInterface = Cast<IAttributeInterface>(InActor);
	if (AttributeInterface != nullptr)
	{
		AttributeInterface->SetAttributeByEnum(InAttribute,InValue, InAttributeValueType);
	}
}

float UAttributeSubsystem::GetAttributeByEnum(AActor* InActor, EAttribute InAttribute, ECAVType InAttributeValueType)
{
	IAttributeInterface* AttributeInterface=Cast<IAttributeInterface>(InActor);
	if (AttributeInterface != nullptr)
	{
		return AttributeInterface->GetAttributeByEnum(InAttribute, InAttributeValueType);
	}

	return 0.0f;
}

void UAttributeSubsystem::SetAttributeByEnum(AActor* InActor, EAttribute InAttribute, float InValue, ECAVType InAttributeValueType)
{
	if (!IsValid(AttributeProxy)) { return; }
	ENetMode CurrentNetMode = GetWorld()->GetNetMode();
	if (CurrentNetMode == NM_DedicatedServer || CurrentNetMode == NM_ListenServer)
	{
		Server_SetAttributeByEnum(InActor, InAttribute, InValue, InAttributeValueType);
	}
	else
	{
		AttributeProxy->Server_SetAttributeByEnum(InActor, InAttribute, InValue, InAttributeValueType);
	}

}
