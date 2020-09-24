// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GauntletTypes.generated.h"

class UModificationItem;
class USpellItem;

/**
 * 
 */

USTRUCT(BlueprintType)
struct MAGICTD_API FSpellConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	USpellItem* Spell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	TArray<UModificationItem*> ModificationList;
};

USTRUCT(BlueprintType)
struct MAGICTD_API FSerializableSpellConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	FPrimaryAssetId Spell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	TArray<FPrimaryAssetId> ModificationList;
};

UENUM(BlueprintType)
enum class ESocketType : uint8
{
	Teleport,
	Enchant,
	Trigger,
	TriggerMulti,
	Grip,
	GripMulti
};

USTRUCT(BlueprintType)
struct MAGICTD_API FGauntletConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	TMap<ESocketType, FSpellConfig> SpellMap;

};
