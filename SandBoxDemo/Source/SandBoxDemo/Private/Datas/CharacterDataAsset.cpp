// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/CharacterDataAsset.h"

FCharacterAttribute& UCharacterDataAsset::GetAttributeByEnum(ECharacterAttribute CharacterAttribute)
{
    checkf(Attributes.Find(CharacterAttribute), TEXT("Attribute %d  Is Not Exist"), CharacterAttribute);

    return Attributes[CharacterAttribute];
    // TODO: 在此处插入 return 语句
}

TSubclassOf<UGameplayAbility> UCharacterDataAsset::GetAbility(FGameplayTag AbilityTag)
{
    checkf(Abilities.Find(AbilityTag), TEXT("Ability %s  Is Not Exist"), *AbilityTag.GetTagName().ToString());
    return Abilities[AbilityTag];
}

bool UCharacterDataAsset::IsVaildAttribute(ECharacterAttribute CharacterAttribute)
{

    return Attributes.Find(CharacterAttribute)?true:false;

}

bool UCharacterDataAsset::IsVaildAbility(FGameplayTag CharacterAttribute)
{

    if (Abilities.Find(CharacterAttribute))
    {
        return true;
    }
    else
    {
        return false;
    }
}

UCharacterDataAsset* UCharacterDataAsset::DeepCopy(UCharacterDataAsset* Source)
{
    UCharacterDataAsset* NewDataAsset = DuplicateObject<UCharacterDataAsset>(Source, GetTransientPackage());
    if (NewDataAsset && Source)
    {
        NewDataAsset->Attributes = Source->Attributes;
    }
    return NewDataAsset;
}
