// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "MyAssetManager.h"
#include "Kismet/GameplayStatics.h"

UMyGameInstance::UMyGameInstance()
	: SaveSlot(TEXT("SaveGame"))
	, SaveUserIndex(0)
{ }

void UMyGameInstance::AddDefaultConfigs(UMySaveGame* SaveGame, bool bRemoveExtra)
{
	if (bRemoveExtra)
	{
		SaveGame->LeftGauntlet.Reset();
		SaveGame->RightGauntlet.Reset();
	}

	for (TPair<ESocketType, FSerializableSpellConfig> DefaultSpell : DefaultLeft)
	{
		if (!SaveGame->LeftGauntlet.Contains(DefaultSpell.Key))
		{
			SaveGame->LeftGauntlet.Add(DefaultSpell.Key, DefaultSpell.Value);
		}
	}

	for (TPair<ESocketType, FSerializableSpellConfig> DefaultSpell : DefaultRight)
	{
		if (!SaveGame->RightGauntlet.Contains(DefaultSpell.Key))
		{
			SaveGame->RightGauntlet.Add(DefaultSpell.Key, DefaultSpell.Value);
		}
	}
}

UMySaveGame* UMyGameInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void UMyGameInstance::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = bEnabled;
}

bool UMyGameInstance::LoadOrCreateSaveGame()
{
	// Drop reference to old save game, this will GC out
	CurrentSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && bSavingEnabled)
	{
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	if (CurrentSaveGame)
	{
		// Make sure it has any newly added default inventory
		AddDefaultConfigs(CurrentSaveGame, false);

		return true;
	}
	else
	{
		// This creates it on demand
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		if (CurrentSaveGame)
		{
			AddDefaultConfigs(CurrentSaveGame, true);
			return true;
		}

		return false;
	}
}

bool UMyGameInstance::WriteSaveGame()
{
	if (bSavingEnabled)
	{
		return UGameplayStatics::SaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex);
	}
	return false;
}

void UMyGameInstance::ResetSaveGame()
{
	bool bWasSavingEnabled = bSavingEnabled;
	bSavingEnabled = false;
	LoadOrCreateSaveGame();
	bSavingEnabled = bWasSavingEnabled;
}
