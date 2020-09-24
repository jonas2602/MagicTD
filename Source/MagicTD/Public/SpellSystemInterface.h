// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpellSystemInterface.generated.h"

class USpellSystemComponent;
class AMyMotionController;
class UAnimInstance;
class USceneComponent;
class AActor;
class USkillBase;
class AGauntlet;
class UJuwelComponentBase;
//class AInteractableBase;

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class USpellSystemInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 *
 */
class MAGICTD_API ISpellSystemInterface // TODO: Add To Motion Controller
{
	GENERATED_IINTERFACE_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpellSystem")
	USpellSystemComponent* GetSpellSystemComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpellSystem")
	bool Overlapping(AMyMotionController*& OverlappingController);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpellSystem")
	AGauntlet* GetGauntlet();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpellSystem")
	bool GetJuwelsToSpell(TSubclassOf<USkillBase> SpellClass, UJuwelComponentBase*& OutSpellJuwel, TArray<UJuwelComponentBase*>& OutModJuwels);

	/*UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpellSystem")
	UAnimInstance* GetAnimBP();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpellSystem")
	USceneComponent* GetSpellSocket();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpellSystem")
	bool GetInteractable(AActor*& Interactable);*/
};
