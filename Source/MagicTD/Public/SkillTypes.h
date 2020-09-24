// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillTypes.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct MAGICTD_API FSkillAttribute
{
	GENERATED_BODY()

	/** Constructor */
	FSkillAttribute()
		: AttributeName("")
		, AttributeValue(0)
		, AttributeMinValue(0)
		, AttributeMaxValue(99999999)
	{}

	FSkillAttribute(FName InAttributeName)
		: AttributeName(InAttributeName)
		, AttributeValue(0)
		, AttributeMinValue(0)
		, AttributeMaxValue(99999999)
	{}

	FSkillAttribute(FName InAttributeName, float InAttributeValue, float inMin, float inMax)
		: AttributeName(InAttributeName)
		, AttributeValue(InAttributeValue)
		, AttributeMinValue(inMin)
		, AttributeMaxValue(inMax)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FName AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	float AttributeValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	float AttributeMinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	float AttributeMaxValue;

	/** Equality operators */
	bool operator==(const FSkillAttribute& Other) const
	{
		return AttributeName == Other.AttributeName;
	}
	bool operator!=(const FSkillAttribute& Other) const
	{
		return !(*this == Other);
	}

	bool IsValid() const
	{
		return AttributeName!= "" && AttributeValue >= 0;
	}

	static const FName Damage_AttributeName;
	static const FName SkillRange_AttributeName;
	static const FName ExplosionRange_AttributeName;
	static const FName ProjectileSpeed_AttributeName;
	static const FName ProjectileCount_AttributeName;
	static const FName Efficiency_AttributeName;
	static const FName Toggle_AttributeName;
};

UENUM(BlueprintType)
enum class EAttributeModifierType : uint8
{
	Add,
	Override
};

USTRUCT(BlueprintType)
struct MAGICTD_API FSkillAttributeModifier
{
	GENERATED_BODY()

	/** Constructor */
	FSkillAttributeModifier()
		: AttributeName("")
		, AttributeDelta(0)
		, AttributeModifierType(EAttributeModifierType::Add)
	{}

	FSkillAttributeModifier(FName InAttributeName, float InAttributeDelta)
		: AttributeName(InAttributeName)
		, AttributeDelta(InAttributeDelta)
		, AttributeModifierType(EAttributeModifierType::Add)
	{}

	FSkillAttributeModifier(FName InAttributeName, float InAttributeDelta, EAttributeModifierType InAttributeModifierType)
		: AttributeName(InAttributeName)
		, AttributeDelta(InAttributeDelta)
		, AttributeModifierType(InAttributeModifierType)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FName AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	float AttributeDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	EAttributeModifierType AttributeModifierType;
};

UCLASS(DefaultToInstanced, Blueprintable, BlueprintType)
class MAGICTD_API USkillAttributeSet : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool Contains(FSkillAttribute Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool FindByName(FName AttributeName, FSkillAttribute& OutAttribute);

	UFUNCTION(BlueprintPure, Category = "Attribute")
	bool GetValue(FName AttributeName, float& OutValue);

	/** Add new Attribute */
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool Insert(FSkillAttribute Attribute);

	/** Edit existing Attribute */
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	bool Adjust(FSkillAttributeModifier Attribute);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	TArray<FSkillAttribute> AttributeList;

private:

	/** Getter for default Skill Attribute Names */
	UFUNCTION(BlueprintPure, Category = "Attribute|Name", meta = (DisplayName = "Damage"))
	static FName GetDamage_AttributeName() { return FSkillAttribute::Damage_AttributeName; }

	UFUNCTION(BlueprintPure, Category = "Attribute|Name", meta = (DisplayName = "SkillRange"))
	static FName GetSkillRange_AttributeName() { return FSkillAttribute::SkillRange_AttributeName; }

	UFUNCTION(BlueprintPure, Category = "Attribute|Name", meta = (DisplayName = "ExplosionRange"))
	static FName GetExplosionRange_AttributeName() { return FSkillAttribute::ExplosionRange_AttributeName; }

	UFUNCTION(BlueprintPure, Category = "Attribute|Name", meta = (DisplayName = "ProjectileSpeed"))
	static FName GetProjectileSpeed_AttributeName() { return FSkillAttribute::ProjectileSpeed_AttributeName; }

	UFUNCTION(BlueprintPure, Category = "Attribute|Name", meta = (DisplayName = "ProjectileCount"))
	static FName GetProjectileCount_AttributeName() { return FSkillAttribute::ProjectileCount_AttributeName; }

	UFUNCTION(BlueprintPure, Category = "Attribute|Name", meta = (DisplayName = "Efficiency"))
	static FName GetEfficiency_AttributeName() { return FSkillAttribute::Efficiency_AttributeName; }

	UFUNCTION(BlueprintPure, Category = "Attribute|Name", meta = (DisplayName = "Toggle"))
	static FName GetToggle_AttributeName() { return FSkillAttribute::Toggle_AttributeName; }
};