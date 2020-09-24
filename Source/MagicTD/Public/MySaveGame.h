// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GauntletTypes.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MAGICTD_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMySaveGame();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TMap<ESocketType, FSerializableSpellConfig> LeftGauntlet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TMap<ESocketType, FSerializableSpellConfig> RightGauntlet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	FString UserId;
};
