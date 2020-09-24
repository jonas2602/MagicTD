// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAssetManager.h"
#include "Engine/Engine.h"
#include "ItemBase.h"

const FPrimaryAssetType	UMyAssetManager::SpellItemType = TEXT("Spell");
const FPrimaryAssetType	UMyAssetManager::ModificationItemType = TEXT("Modification");

UMyAssetManager& UMyAssetManager::Get()
{
	UMyAssetManager* This = Cast<UMyAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogClass, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be MyAssetManager!"));
		return *NewObject<UMyAssetManager>(); // never calls this
	}
}

UItemBase* UMyAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UItemBase* LoadedItem = Cast<UItemBase>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}


