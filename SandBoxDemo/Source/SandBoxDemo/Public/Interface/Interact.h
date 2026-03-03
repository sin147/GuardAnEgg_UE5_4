// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Interact.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SANDBOXDEMO_API IInteract
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//是否可以交互
	virtual bool CanInteract(ACharacter* InCharacter) 
	{
		if (InCharacter == nullptr)
		{
			return false;
		}

		return true;
	}

	//交互接口
	virtual void Interact(ACharacter* InCharacter)= 0;
	
};
