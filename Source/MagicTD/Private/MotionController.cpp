// Fill out your copyright notice in the Description page of Project Settings.

#include "MotionController.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyGameInstance.h"
#include "MySaveGame.h"
#include "MyAssetManager.h"
#include "Engine/World.h"
#include "Gauntlet/Items/SpellItem.h"
#include "Gauntlet/Items/ModificationItem.h"

// Sets default values
AMyMotionController::AMyMotionController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->SetupAttachment(RootComponent);
	MotionController->SetTrackingSource(Hand);

	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(MotionController);
	HandMesh->SetRelativeLocationAndRotation(FVector(-12.785664f, 0.028015f, -1.789026f), FRotator(0.0f, 0.0f, 90.0f));

	ProjectileSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSocket"));
	ProjectileSocket->SetupAttachment(HandMesh);
	ProjectileSocket->SetRelativeLocationAndRotation(FVector(14.0f, 0.0f, -4.0f), FRotator(180.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AMyMotionController::BeginPlay()
{
	//LoadGauntletConfig();

	Super::BeginPlay();

}

// Called every frame
void AMyMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool AMyMotionController::SaveGauntletConfig(FGauntletConfig GauntletConfig)
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

	Hand == EControllerHand::Left 
		? CurrentSaveGame->LeftGauntlet.Reset() 
		: CurrentSaveGame->RightGauntlet.Reset();

	for (TPair<ESocketType, FSpellConfig> SpellConfig : GauntletConfig.SpellMap)
	{
		FSerializableSpellConfig SerializableSpell = FSerializableSpellConfig();
		if (SpellConfig.Value.Spell)
		{
			SerializableSpell.Spell = SpellConfig.Value.Spell->GetPrimaryAssetId();
		}

		for (UModificationItem* Mod : SpellConfig.Value.ModificationList)
		{
			if (Mod)
			{
				SerializableSpell.ModificationList.Add(Mod->GetPrimaryAssetId());
			}
		}

		Hand == EControllerHand::Left 
			? CurrentSaveGame->LeftGauntlet.Add(SpellConfig.Key, SerializableSpell)
			: CurrentSaveGame->RightGauntlet.Add(SpellConfig.Key, SerializableSpell);
	}

	// Now that cache is updated, write to disk
	return GameInstance->WriteSaveGame();
}

bool AMyMotionController::LoadGauntletConfig(FGauntletConfig& GauntletConfig)
{
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

	for (TPair<ESocketType, FSerializableSpellConfig> SerializedSpell : Hand == EControllerHand::Left ? CurrentSaveGame->LeftGauntlet : CurrentSaveGame->RightGauntlet)
	{
		FSpellConfig SpellConfig = FSpellConfig();
		USpellItem* LoadedSpell = Cast<USpellItem>(AssetManager.ForceLoadItem(SerializedSpell.Value.Spell));
		SpellConfig.Spell = LoadedSpell;

		for (FPrimaryAssetId ModId : SerializedSpell.Value.ModificationList)
		{
			UModificationItem* LoadedMod = Cast<UModificationItem>(AssetManager.ForceLoadItem(ModId));
			SpellConfig.ModificationList.Add(LoadedMod);
		}

		GauntletConfig.SpellMap.Add(SerializedSpell.Key, SpellConfig);
	}

	return true;
}