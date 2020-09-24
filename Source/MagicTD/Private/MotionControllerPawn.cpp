// Fill out your copyright notice in the Description page of Project Settings.

#include "MotionControllerPawn.h"
#include "SkillBase.h"
#include "MyGameInstance.h"
#include "MySaveGame.h"
#include "MyAssetManager.h"
#include "Gauntlet/Items/SpellItem.h"
#include "Gauntlet/Items/ModificationItem.h"
#include "Engine/World.h"


// Sets default values
AMotionControllerPawn::AMotionControllerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMotionControllerPawn::BeginPlay()
{
	LoadGauntletConfig();

	Super::BeginPlay();
	
}

UActorComponent* AMotionControllerPawn::AddActorComponent(TSubclassOf<UActorComponent> ComponentClass, FName Name)
{
	UActorComponent* NewComp = NewObject<UActorComponent>(this, ComponentClass, Name);
	if (!NewComp)
	{
		UE_LOG(LogClass, Error, TEXT("Component invalid"));
		return nullptr;
	}

	AddOwnedComponent(NewComp);
	return NewComp;
}

// Called every frame
void AMotionControllerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMotionControllerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


bool AMotionControllerPawn::SaveGauntletConfig()
{
	UMyGameInstance* GameInstance = GetWorld()->GetGameInstance<UMyGameInstance>();
	if (!GameInstance)
	{
		return false;
	}

	UMySaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (!CurrentSaveGame)
	{
		return false;
	}

	CurrentSaveGame->LeftGauntlet.Reset();
	CurrentSaveGame->RightGauntlet.Reset();

	for (TPair<ESocketType, FSpellConfig> SpellConfig : LeftGauntletConfig.SpellMap)
	{
		FSerializableSpellConfig SerializableSpell = FSerializableSpellConfig();
		SerializableSpell.Spell = SpellConfig.Value.Spell->GetPrimaryAssetId();

		for (UModificationItem* Mod : SpellConfig.Value.ModificationList)
		{
			SerializableSpell.ModificationList.Add(Mod->GetPrimaryAssetId());
		}

		CurrentSaveGame->LeftGauntlet.Add(SpellConfig.Key, SerializableSpell);
	}

	for (TPair<ESocketType, FSpellConfig> SpellConfig : RightGauntletConfig.SpellMap)
	{
		FSerializableSpellConfig SerializableSpell = FSerializableSpellConfig();
		SerializableSpell.Spell = SpellConfig.Value.Spell->GetPrimaryAssetId();

		for (UModificationItem* Mod : SpellConfig.Value.ModificationList)
		{
			SerializableSpell.ModificationList.Add(Mod->GetPrimaryAssetId());
		}

		CurrentSaveGame->RightGauntlet.Add(SpellConfig.Key, SerializableSpell);
	}

	// Now that cache is updated, write to disk
	return GameInstance->WriteSaveGame();
}

bool AMotionControllerPawn::LoadGauntletConfig()
{
	LeftGauntletConfig = FGauntletConfig();
	RightGauntletConfig = FGauntletConfig();

	UMyGameInstance* GameInstance = GetWorld()->GetGameInstance<UMyGameInstance>();
	if (!GameInstance)
	{
		return false;
	}

	UMySaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	UMyAssetManager& AssetManager = UMyAssetManager::Get();
	if (!CurrentSaveGame)
	{
		return false;
	}

	for (TPair<ESocketType, FSerializableSpellConfig> SerializedSpell : CurrentSaveGame->LeftGauntlet)
	{
		FSpellConfig SpellConfig = FSpellConfig();
		USpellItem* LoadedSpell = Cast<USpellItem>(AssetManager.ForceLoadItem(SerializedSpell.Value.Spell));
		SpellConfig.Spell = LoadedSpell;

		for (FPrimaryAssetId ModId : SerializedSpell.Value.ModificationList)
		{
			UModificationItem* LoadedMod = Cast<UModificationItem>(AssetManager.ForceLoadItem(ModId));
			SpellConfig.ModificationList.Add(LoadedMod);
		}

		LeftGauntletConfig.SpellMap.Add(SerializedSpell.Key, SpellConfig);
	}

	for (TPair<ESocketType, FSerializableSpellConfig> SerializedSpell : CurrentSaveGame->RightGauntlet)
	{
		FSpellConfig SpellConfig = FSpellConfig();
		USpellItem* LoadedSpell = Cast<USpellItem>(AssetManager.ForceLoadItem(SerializedSpell.Value.Spell));
		SpellConfig.Spell = LoadedSpell;

		for (FPrimaryAssetId ModId : SerializedSpell.Value.ModificationList)
		{
			UModificationItem* LoadedMod = Cast<UModificationItem>(AssetManager.ForceLoadItem(ModId));
			SpellConfig.ModificationList.Add(LoadedMod);
		}

		RightGauntletConfig.SpellMap.Add(SerializedSpell.Key, SpellConfig);
	}

	return true;
}



