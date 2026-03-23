// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/SandBoxAnimNotify_Base.h"
void USandBoxAnimNotify_Base::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
 	
	ASandBoxPeople*People=Cast<ASandBoxPeople> (MeshComp->GetAttachParentActor());
	if (IsValid(People))
	{
		SandBoxNotify(People);
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("SandboxNotify只能用于ASandboxPeople中"));
	}
}

void USandBoxAnimNotify_Base::SandBoxNotify(ASandBoxPeople* SandBoxPeople)
{
}
