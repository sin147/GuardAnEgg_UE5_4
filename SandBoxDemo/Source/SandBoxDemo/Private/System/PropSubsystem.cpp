// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PropSubsystem.h"

UPropSubsystem::UPropSubsystem()
{
	UE_LOG(LogTemp, Log, TEXT("Inital PropSubsystem"));
	PropDataAsset = LoadObject<UPropDataAsset>(nullptr, TEXT("/Game/Datas/PropDatas.PropDatas"));
	if (IsValid(PropDataAsset))
	{
		PropDataAsset = DuplicateObject<UPropDataAsset>(PropDataAsset, UPropSubsystem::StaticClass());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PropSubsystem:Do Not Set PropDataAsset"));
	}
}

bool UPropSubsystem::DropProp(int PropID, FVector InLocation)
{
	if (IsValid(PropDataAsset) and PropDataAsset->PropConfig.Find(PropID))
	{
		APropBase* NewProp=GetWorld()->SpawnActorDeferred<APropBase>(PropDataAsset->PropConfig[PropID].PropClass,FTransform(FRotator(FMath::Rand() % 360, 0, 0),InLocation));
		if (NewProp)
		{
			Props.Add(NewProp->GetGUID(), NewProp);
			NewProp->InitInfo(PropID);
			NewProp->FinishSpawning(FTransform(FRotator(FMath::Rand() % 360, 0, 0), InLocation));
			return true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UPropSubsystem:Can not Spawn Prop:%d"), PropID);
	}
	UE_LOG(LogTemp, Log, TEXT("UPropSubsystem:faild Drop Prop:%d"), PropID);
	return false;
}

void UPropSubsystem::DeleteProp(FGuid PropGUID)
{
	if (Props.Find(PropGUID))
	{
		Props[PropGUID]->Destroy();
		Props.Remove(PropGUID);
	}

}

APropBase* UPropSubsystem::GetProp(FGuid PropGUID)
{
	if (Props.Find(PropGUID))
	{
		return Props[PropGUID];
	}
	return nullptr;
}
