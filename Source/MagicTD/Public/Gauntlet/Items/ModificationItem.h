// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gauntlet/Items/JuwelItem.h"
#include "SkillTypes.h"
#include "ModificationItem.generated.h"

class USkillModifierBase;

/**
 * 
 */
UCLASS()
class MAGICTD_API UModificationItem : public UJuwelItem
{
	GENERATED_BODY()
	
public:
	UModificationItem();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Modification")
	TArray<FSkillAttributeModifier> AttributeModifierList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modification")
	TSubclassOf<USkillModifierBase> FunctionalModifierClass;
};
