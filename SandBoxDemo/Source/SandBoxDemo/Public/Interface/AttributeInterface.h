// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "System/AttributeSubsystem.h"
#include "AttributeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttributeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SANDBOXDEMO_API IAttributeInterface
{
	GENERATED_BODY()
	friend class UAttributeSubsystem;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:

	virtual void SetAttributeByEnum(EAttribute InAttribute,float Value,ECAVType InAttributeValueType=ECAVType::CAVT_Currently)=0;

	virtual float GetAttributeByEnum(EAttribute InAttribute, ECAVType InAttributeValueType = ECAVType::CAVT_Currently) = 0;

public:
	
	
};
