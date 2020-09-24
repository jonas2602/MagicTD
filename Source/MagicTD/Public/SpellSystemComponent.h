// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GauntletTypes.h"
#include "SpellSystemComponent.generated.h"

class USkillBase;
class AActor;
class AMyMotionController;


UENUM(BlueprintType)
enum class ESlotType : uint8
{
	Stick,
	Grip,
	Trigger
};

UCLASS(Abstract, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGICTD_API USpellSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USpellSystemComponent();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void InitAbiltiyActorInfo(AActor* Owner, AMyMotionController* Controller);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool ConnectSystems() { return false; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Controller nullptr means on both
	UFUNCTION(BlueprintCallable, Category = "Default")
	void GiveSpell(FSpellConfig SpellConfig); // TODO: SlotType?

	UFUNCTION(BlueprintCallable, Category = "Default")
	void RemoveSpell(FSpellConfig SpellConfig);
	
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ClearSpells();

	// TODO: Activate Spell by Slot? Decide Single/Multi/Support later?
	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryActivateSpell(TSubclassOf<USkillBase> SpellClass); // Deprecated

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryReleaseSpell(TSubclassOf<USkillBase> SpellClass); // Deprecated

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryActivateSpellBySlot(TSubclassOf<USkillBase> SpellClass, ESlotType Slot);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TrySupportSpellBySlot(AMyMotionController* OtherController, ESlotType Slot);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryReleaseSpellBySlot(ESlotType Slot);

protected:
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ResetActiveSpell(USkillBase* Spell);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Default")
	bool CanUseSpell(USkillBase* Skill);
	virtual bool CanUseSpell_Implementation(USkillBase* Skill);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool GetOverlappingSpell(AMyMotionController* OtherController, USkillBase*& OtherSpell);

	UPROPERTY(BlueprintReadWrite, Category = "Default")
	AActor* Owner;

	UPROPERTY(BlueprintReadWrite, Category = "Default")
	AMyMotionController* Controller;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	USkillBase* ActiveSpell;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	ESlotType ActiveSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	bool Supporting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	TMap<TSubclassOf<USkillBase>, USkillBase*> Spells; // TODO: Rename to SpellStorage 

	// TODO: BlockType + Skill as Reference throught ConnectSystems()
};
