// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gauntlet/Items/JuwelItem.h"
#include "SpellItem.generated.h"

class USkillBase;
class UStaticMesh;

/**
 * 
 */
UCLASS()
class MAGICTD_API USpellItem : public UJuwelItem
{
	GENERATED_BODY()
	
public:
	USpellItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	TSubclassOf<USkillBase> SpellClass;
};
