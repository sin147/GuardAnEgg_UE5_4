// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/CharacterAttributeDataAsset.h"

FCharacterAttribute& UCharacterAttributeDataAsset::operator[](ECharacterAttribute CharacterAttribute)
{

    checkf(Attributes.Find(CharacterAttribute), TEXT("Attribute %d  Is Not Exist"), CharacterAttribute);

    return Attributes[CharacterAttribute];
    // TODO: 在此处插入 return 语句
}

bool UCharacterAttributeDataAsset::IsVaildKey(ECharacterAttribute CharacterAttribute)
{

    return Attributes.Find(CharacterAttribute)?true:false;

}
