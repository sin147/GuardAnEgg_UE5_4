// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Peoples/SandBoxPeople_Soldier.h"
#include "SBPeople_TankerSoldier.generated.h"
class UNiagaraComponent;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class SANDBOXDEMO_API ASBPeople_TankerSoldier : public ASandBoxPeople_Soldier
{
	GENERATED_BODY()
public:
	ASBPeople_TankerSoldier();
	virtual void BeginPlay() override;
	//对目标应用技能
	void ApplySkillToPeople(ASandBoxPeople*InPeople);

protected:
	

	//已生成的Niagara组件和对应Actor
	UPROPERTY(EditAnywhere, Category = "People|Skill")
	TMap<FString,UNiagaraComponent*> NiagaraComponents;
	////防御图标
	UPROPERTY(EditAnywhere, Category = "People|Skill")
	UNiagaraSystem* NSDefense;
	//技能影响半径
	UPROPERTY(EditAnywhere, Category = "People|Skill")
	float SkillRadius = 300.0f;
	//技能碰撞框
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere, Category = "People|Skill")
	class USphereComponent* SkillCollisionComp;
	//技能碰撞框开始重叠事件
	UFUNCTION()
	void OnSkillCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//技能碰撞框结束重叠事件
	UFUNCTION()
	void OnSkillCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
