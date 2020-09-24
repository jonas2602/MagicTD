// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractorInterface.generated.h"

class UAnimInstance;
class USceneComponent;
class AActor;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONSYSTEM_API IInteractorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactor")
	UAnimInstance* GetAnimBP();
	virtual UAnimInstance* GetAnimBP_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactor")
	USceneComponent* GetPickupParent(FName& OutSocketName);
	virtual USceneComponent*GetPickupParent_Implementation(FName& OutSocketName);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactor")
	FVector GetSocketLocation();
	virtual FVector GetSocketLocation_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactor")
	AActor* GetActiveInteractable();
	virtual AActor* GetActiveInteractable_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactor")
	void ForceRelease(AActor* Interactable);
	virtual void ForceRelease_Implementation(AActor* Interactable);
};
