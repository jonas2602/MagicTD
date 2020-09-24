// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractorInterface.h"
#include "InteractionSystem.h"


// Add default functionality here for any IInteractorInterface functions that are not pure virtual.
UAnimInstance* IInteractorInterface::GetAnimBP_Implementation()
{
	UE_LOG(LogInteraction, Error, TEXT("GetAnimBP not implemented"));
	return nullptr;
}

USceneComponent* IInteractorInterface::GetPickupParent_Implementation(FName& OutSocketName)
{
	UE_LOG(LogInteraction, Error, TEXT("GetPickupParent not implemented"));
	return nullptr;
}

FVector IInteractorInterface::GetSocketLocation_Implementation()
{
	UE_LOG(LogInteraction, Error, TEXT("GetSocketLocation not implemented"));
	return FVector();
}

AActor* IInteractorInterface::GetActiveInteractable_Implementation()
{
	UE_LOG(LogInteraction, Error, TEXT("GetActiveInteractable not implemented"));
	return nullptr;
}

void IInteractorInterface::ForceRelease_Implementation(AActor* Interactable)
{
	UE_LOG(LogInteraction, Error, TEXT("ForceRelease not implemented"));
	return;
}