// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillTypes.h"

const FName FSkillAttribute::Damage_AttributeName = FName(TEXT("Damage"));
const FName FSkillAttribute::SkillRange_AttributeName = FName(TEXT("SkillRange"));
const FName FSkillAttribute::ExplosionRange_AttributeName = FName(TEXT("ExplosionRange"));
const FName FSkillAttribute::ProjectileSpeed_AttributeName = FName(TEXT("ProjectileSpeed"));
const FName FSkillAttribute::ProjectileCount_AttributeName = FName(TEXT("ProjectileCount"));
const FName FSkillAttribute::Efficiency_AttributeName = FName(TEXT("Efficiency"));
const FName FSkillAttribute::Toggle_AttributeName = FName(TEXT("Toggle"));

bool USkillAttributeSet::Contains(FSkillAttribute Attribute)
{
	return AttributeList.Contains(Attribute.AttributeName);
}

bool USkillAttributeSet::FindByName(FName AttributeName, FSkillAttribute& OutAttribute)
{
	int Index;
	FSkillAttribute TempAttribute(AttributeName);
	if (!AttributeList.Find(TempAttribute, Index))
	{
		UE_LOG(LogClass, Warning, TEXT("%s not in AttributeSet"), *AttributeName.ToString());
		return false;
	}

	OutAttribute = AttributeList[Index];
	return true;
}

bool USkillAttributeSet::GetValue(FName AttributeName, float& OutValue)
{
	FSkillAttribute Attribute;
	if (FindByName(AttributeName, Attribute))
	{
		OutValue = Attribute.AttributeValue;
		return true;
	}

	return false;
}

bool USkillAttributeSet::Insert(FSkillAttribute Attribute)
{
	if (Contains(Attribute)) return false;

	AttributeList.Add(Attribute);
	return true;
}

bool USkillAttributeSet::Adjust(FSkillAttributeModifier AttributeModifier)
{
	int Index;
	FSkillAttribute TempAttribute(AttributeModifier.AttributeName);
	if (AttributeList.Find(TempAttribute, Index))
	{
		FSkillAttribute FoundAttribute = AttributeList[Index];

		switch (AttributeModifier.AttributeModifierType) {
			case EAttributeModifierType::Add: {
				FoundAttribute.AttributeValue += AttributeModifier.AttributeDelta;
				break;
			}
			case EAttributeModifierType::Override: {
				FoundAttribute.AttributeValue = AttributeModifier.AttributeDelta;
				break;
			}
		}
		FoundAttribute.AttributeValue = FMath::Clamp(FoundAttribute.AttributeValue, FoundAttribute.AttributeMinValue, FoundAttribute.AttributeMaxValue);

		AttributeList[Index] = FoundAttribute;
		return true;
	}

	if (AttributeModifier.AttributeModifierType == EAttributeModifierType::Override)
	{
		AttributeList.Add(FSkillAttribute(AttributeModifier.AttributeName, AttributeModifier.AttributeDelta, 0, 100000));
	}

	return false;
}
