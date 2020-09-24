// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellSystemComponent.h"
#include "MotionController.h"
#include "SupportableSkillBase.h"
#include "SpellItem.h"
#include "ModificationItem.h"
#include "SpellSystemInterface.h"


// Sets default values for this component's properties
USpellSystemComponent::USpellSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USpellSystemComponent::InitAbiltiyActorInfo(AActor* Owner, AMyMotionController* Controller)
{
	this->Owner = Owner;
	this->Controller = Controller;
}

// Called when the game starts
void USpellSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void USpellSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActiveSpell)
	{
		ActiveSpell->TickSkill(DeltaTime);
	}
}

void USpellSystemComponent::GiveSpell(FSpellConfig SpellConfig)
{
	if (!SpellConfig.Spell || !SpellConfig.Spell->SpellClass)
	{
		UE_LOG(LogClass, Error, TEXT("Invalid Spellclass"));
		return;
	}

	TSubclassOf<USkillBase> SkillClass = SpellConfig.Spell->SpellClass;
	UE_LOG(LogClass, Error, TEXT("Give %s"), *SkillClass->GetName());

	USkillBase* SkillInstance = NewObject<USkillBase>(this, SkillClass);
	SkillInstance->Init();
	Spells.Add(SkillClass, SkillInstance);
	 

	for (UModificationItem* Modifier : SpellConfig.ModificationList)
	{
		if (!SkillInstance->ApplyModifier(Modifier))
		{
			UE_LOG(LogClass, Error, TEXT("Failed to Apply %s"), *Modifier->GetName())
		}
	}
}

void USpellSystemComponent::RemoveSpell(FSpellConfig SpellConfig)
{
	Spells.Remove(SpellConfig.Spell->SpellClass);
}

void USpellSystemComponent::ClearSpells()
{
	Spells.Reset();
}

/******************************* End Component Setup *******************************/

bool USpellSystemComponent::TryActivateSpellBySlot(TSubclassOf<USkillBase> SpellClass, ESlotType Slot)
{
	if (!SpellClass) return false;

	if (!Spells.Contains(SpellClass))
	{
		UE_LOG(LogClass, Error, TEXT("Spell not Available"));
		return false;
	}

	USkillBase* Spell = *Spells.Find(SpellClass);
	if (!Spell)
	{
		UE_LOG(LogClass, Error, TEXT("Spell for Class doesn't exist"));
		return false;
	}

	if (ActiveSpell)
	{
		UE_LOG(LogClass, Error, TEXT("Unable to activate Spells at the moment"));
		return false;
	}

	Spell->ActivateSkill(Controller);

	this->ActiveSpell = Spell;
	this->ActiveSlot = Slot;

	Spell->OnFinished.AddDynamic(this, &USpellSystemComponent::ResetActiveSpell);

	return true;
}

bool USpellSystemComponent::TrySupportSpellBySlot(AMyMotionController* OtherController, ESlotType Slot)
{
	if (ActiveSpell) return false;

	USkillBase* OtherSpell;
	if (!GetOverlappingSpell(OtherController, OtherSpell))
	{
		UE_LOG(LogClass, Error, TEXT("Other Controller not using a spell"));
		return false;
	}

	USupportableSkillBase* SupportableSpell = Cast<USupportableSkillBase>(OtherSpell);
	if (!SupportableSpell)
	{
		UE_LOG(LogClass, Warning, TEXT("Spell is not Supportable"));
		return false;
	}

	SupportableSpell->ActivateSupport(Controller);
	SupportableSpell->OnSupportFinished.AddDynamic(this, &USpellSystemComponent::ResetActiveSpell);

	this->ActiveSpell = SupportableSpell;
	this->ActiveSlot = Slot;
	this->Supporting = true;

	return true;
}

bool USpellSystemComponent::TryReleaseSpellBySlot(ESlotType Slot)
{
	if (!ActiveSpell)
	{
		UE_LOG(LogClass, Error, TEXT("No Active Spell to Release"));
		return false;
	}

	if (ActiveSlot != Slot)
	{
		UE_LOG(LogClass, Warning, TEXT("Trying to release a different spell (%d != %d)"), (int)ActiveSlot, (int)Slot);
		return false;
	}

	if (Supporting)
	{
		Cast<USupportableSkillBase>(ActiveSpell)->ReleaseSupport();
	}
	else
	{
		ActiveSpell->ReleaseSkill();
	}

	return true;
}

bool USpellSystemComponent::TryActivateSpell(TSubclassOf<USkillBase> SpellClass)
{
	/*if (!SpellClass) return false;

	USkillBase* Spell = *Spells.Find(SpellClass);
	if (!Spell)
	{
		UE_LOG(LogClass, Error, TEXT("Spell for Class doesn't exist"));
		return false;
	}

	if (!CanUseSpell(Spell))
	{
		UE_LOG(LogClass, Error, TEXT("Unable to activate Spells at the moment"));
		return false;
	}

	Spell->SkillKeyPress(Controller);
	ActiveSpell = Spell;

	Spell->OnFinished.AddDynamic(this, &USpellSystemComponent::ResetActiveSpell);
*/
	return false;
}

bool USpellSystemComponent::TryReleaseSpell(TSubclassOf<USkillBase> SpellClass)
{
	/*if (!ActiveSpell)
	{
		UE_LOG(LogClass, Error, TEXT("No Active Spell"));
		return false;
	}

	if (ActiveSpell->GetClass() != SpellClass)
	{
		UE_LOG(LogClass, Warning, TEXT("Trying to release a different spell (%s != %s)"), *ActiveSpell->GetClass()->GetName(), *SpellClass->GetName());
		return false;
	}

	ActiveSpell->SkillKeyRelease();*/
	return false;
}

/****************************** Start Helper *************************************/

void USpellSystemComponent::ResetActiveSpell(USkillBase* Spell)
{
	if (!Spell) return;
	// TODO: Check if Spell equals active?

	Spell->OnFinished.RemoveDynamic(this, &USpellSystemComponent::ResetActiveSpell);

	USupportableSkillBase* SupportableSpell = Cast<USupportableSkillBase>(Spell);
	if (SupportableSpell)
	{
		SupportableSpell->OnSupportFinished.RemoveDynamic(this, &USpellSystemComponent::ResetActiveSpell);
	}

	ActiveSpell = nullptr;
	Supporting = false;
}

bool USpellSystemComponent::CanUseSpell_Implementation(USkillBase* SkillClass)
{
	if (ActiveSpell)
	{
		// Already using a skill with this controller
		return false;
	}

	// TODO: Check If spell conditions are fulfilled to activate

	return true;
}

bool USpellSystemComponent::GetOverlappingSpell(AMyMotionController* OtherController, USkillBase*& OtherSpell)
{
	if (!OtherController) return false;

	if (!OtherController->GetClass()->ImplementsInterface(USpellSystemInterface::StaticClass()))
	{
		UE_LOG(LogClass, Error, TEXT("Other controller not implementing spellsystem interface"));
		return false;
	}

	USpellSystemComponent* OtherSpellSystem = ISpellSystemInterface::Execute_GetSpellSystemComponent(OtherController);
	if (!OtherSpellSystem)
	{
		UE_LOG(LogClass, Error, TEXT("Other SpellSystem invalid"));
		return false;
	}

	OtherSpell = OtherSpellSystem->ActiveSpell;
	return true;
}

//bool USpellSystemComponent::TryStopSupportSpell(AMyMotionController* OtherController)
//{
//	USkillBase* OtherSpell;
//	if (!GetOverlappingSpell(OtherController, OtherSpell))
//	{
//		UE_LOG(LogClass, Error, TEXT("Other Controller not using a spell"));
//		return false;
//	}
//
//	if (OtherSpell != ActiveSpell)
//	{
//		UE_LOG(LogClass, Error, TEXT("Spells are not equal"));
//		return false;
//	}
//
//	ActiveSpell->SupportKeyRelease();
//	return true;
//}

