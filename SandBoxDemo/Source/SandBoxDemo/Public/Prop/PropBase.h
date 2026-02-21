// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "PropBase.generated.h"

USTRUCT(BlueprintType)
struct FPropInfo
{
	GENERATED_BODY()
public:
	//道具GUID
	FGuid GUID;
	//道具ID
	int PropID;
};

UCLASS(Blueprintable, CollapseCategories, Config = Propconfig, defaultconfig, configdonotcheckdefaults)
class SANDBOXDEMO_API APropBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APropBase();
	void InitInfo(int InPropID); //TODO
private:
	//GUID

	FPropInfo PropInfo;
public:
	FGuid GetGUID();
	int GetPropID();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//道具网格
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Prop")
	TObjectPtr<UStaticMeshComponent> PropMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
