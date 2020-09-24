// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableInterface.h"
#include "InteractionSystem.h"
#include "InteractionWidgetComponent.h"


// Add default functionality here for any IInteractionInterface functions that are not pure virtual.
//void IInteractionInterface::InitInteractionWidget()
//{
//
//}
//
//void IInteractionInterface::SetupInteractionWidget()
//{
//
//}

bool IInteractableInterface::Interact(AActor* Interactor)
{
	UE_LOG(LogInteraction, Error, TEXT("Interact not implemented")); // "InteractionWidget not Setup or GetInteractionWidget() not implemented"));
	return false;
}

bool IInteractableInterface::Release(AActor* Interactor)
{
	UE_LOG(LogInteraction, Error, TEXT("Release not implemented")); // "InteractionWidget not Setup or GetInteractionWidget() not implemented"));
	return false;
}

void IInteractableInterface::ToggleHighlight(bool Highlight)
{
	// UInteractionWidgetComponent* WidgetComponent = GetInteractionWidget();
	// if (WidgetComponent == nullptr)
	// {
	UE_LOG(LogInteraction, Error, TEXT("Toggle Highlight not implemented")); // "InteractionWidget not Setup or GetInteractionWidget() not implemented"));
	return;
	// }

	// WidgetComponent->SetVisibility(Highlight);
}

FVector IInteractableInterface::GetInteractionLocation()
{
//	UInteractionWidgetComponent* WidgetComponent = GetInteractionWidget();
//	if (WidgetComponent == nullptr)
//	{
	UE_LOG(LogInteraction, Error, TEXT("Get Interaction Location not setup")); // InteractionWidget not Setup or GetInteractionWidget() not implemented"));
	return FVector(0.0f, 0.0f, 0.0f);
//	}
//
//	return WidgetComponent->GetComponentLocation();
}

float IInteractableInterface::GetInteractionMaxDistance()
{
	UE_LOG(LogInteraction, Error, TEXT("Get Interaction Location not setup"));
	return 0;
}