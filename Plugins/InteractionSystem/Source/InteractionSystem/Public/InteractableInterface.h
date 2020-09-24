// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

class UInteractionWidgetComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual	bool Interact(AActor* Interactor);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual	bool Release(AActor* Interactor);
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual	void ToggleHighlight(bool Highlight);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual	FVector GetInteractionLocation();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual	float GetInteractionMaxDistance();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual	bool CanInteract(AActor* Interactor) { return false; }

protected:
	/*UFUNCTION(BlueprintCallable, Category = "UI")
	virtual	void InitInteractionWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual	void SetupInteractionWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual	UInteractionWidgetComponent* GetInteractionWidget() { return nullptr; }*/
};
