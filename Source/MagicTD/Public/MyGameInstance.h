// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MySaveGame.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MAGICTD_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	// Constructor
	UMyGameInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gauntlet")
	TMap<ESocketType, FSerializableSpellConfig> DefaultLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gauntlet")
	TMap<ESocketType, FSerializableSpellConfig> DefaultRight;

	/** The slot name used for saving */
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	FString SaveSlot;

	/** The platform-specific user index */
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	int32 SaveUserIndex;

	/** Adds the default Gauntlet Config to the SaveGame if empty */
	UFUNCTION(BlueprintCallable, Category = "Gauntlet")
	void AddDefaultConfigs(UMySaveGame* SaveGame, bool bRemoveExtra = false);

	/** Returns the current save game, so it can be used to initialize state. Changes are not written until WriteSaveGame is called */
	UFUNCTION(BlueprintCallable, Category = "Save")
	UMySaveGame* GetCurrentSaveGame();

	/** Sets rather save/load is enabled. If disabled it will always count as a new character */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SetSavingEnabled(bool bEnabled);

	/** Loads a save game. If it fails, it will create a new one for you. Returns true if it loaded or successfully created new one */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool LoadOrCreateSaveGame();

	/** Writes the current save game object to disk */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool WriteSaveGame();

	/** Resets the current save game to it's default. This will erase player data! This won't save to disk until the next WriteSaveGame */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void ResetSaveGame();

protected:
	/** The current save game object */
	UPROPERTY()
	UMySaveGame* CurrentSaveGame;

	/** Rather it will attempt to actually save to disk */
	UPROPERTY(EditDefaultsOnly, Category = "Save")
	bool bSavingEnabled;
	
};
