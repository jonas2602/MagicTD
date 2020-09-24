// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonScannerComponent.h"
#include "Camera/CameraComponent.h"

void UThirdPersonScannerComponent::EnableScanner(UCameraComponent* Camera)
{
	// Set Camera ref
	this->OwnerCamera = Camera;

	// Start default initialization
	Super::EnableScanner();
}

void UThirdPersonScannerComponent::EnableScanner()
{
	UE_LOG(LogInteraction, Error, TEXT("Can't enable Scanner without Camera. Please use EnableScannerWithCamera(UCameraComponent*) instead."));
	
	//Super::EnableScanner();
	return;
}

AActor* UThirdPersonScannerComponent::CalcBestTarget(TArray<AActor*> Actors)
{
	AActor* BestTarget = nullptr;
	float BestDotProduct = InteractionDotThreshold;

	for (AActor* Actor : Actors)
	{
		IInteractableInterface* ActorInterface = Cast<IInteractableInterface>(Actor);

		// Allowed to interact with?
		if (!ActorInterface->CanInteract(this->GetOwner()))
		{
			continue;
		}

		// Get Vector to Interactable
		FVector ActorLocation = ActorInterface->GetInteractionLocation();
		FVector CameraLocation = OwnerCamera->GetComponentLocation();
		FVector ToActor = ActorLocation - CameraLocation;
		ToActor.Normalize();

		// Get Camera Forward
		FVector CamForward = OwnerCamera->GetForwardVector();

		// Calculate Dot Product between CamForward and toInteractable
		float DotProduct = FVector::DotProduct(ToActor, CamForward);

		// check if best
		if (DotProduct > BestDotProduct)
		{
			BestTarget = Actor;
			BestDotProduct = DotProduct;
		}
	}

	return BestTarget;
}


