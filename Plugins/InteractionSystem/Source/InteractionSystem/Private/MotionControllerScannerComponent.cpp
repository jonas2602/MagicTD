// Fill out your copyright notice in the Description page of Project Settings.

#include "MotionControllerScannerComponent.h"

// Called when the game starts
void UMotionControllerScannerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (StartWithScannerEnabled)
	{
		EnableScanner();
	}
}

void UMotionControllerScannerComponent::EnableScanner()
{
	Super::EnableScanner();
}

AActor* UMotionControllerScannerComponent::CalcBestTarget(TArray<AActor*> Actors)
{
	AActor* BestTarget = nullptr;
	float BestDistance = 0;

	for (AActor* Actor : Actors)
	{
		IInteractableInterface* ActorInterface = Cast<IInteractableInterface>(Actor);

		// Allowed to interact with?
		if (!ActorInterface->CanInteract(this->GetOwner()))
		{
			// UE_LOG(LogInteraction, Warning, TEXT("CanInteract() returned false"));
			continue;
		}

		// Get Vector to Interactable
		FVector ActorLocation = ActorInterface->GetInteractionLocation();
		FVector CameraLocation = this->GetComponentLocation();
		FVector ToActor = ActorLocation - CameraLocation;

		// Calculate distance from CamForward to toInteractable
		float Distance = FVector::Dist(ActorLocation, CameraLocation);
		float ActorMaxDistance = ActorInterface->GetInteractionMaxDistance();
		if (Distance > ActorMaxDistance && ActorMaxDistance != 0)
		{
			// UE_LOG(LogInteraction, Warning, TEXT("MaxDistance (%f) returned false (%s)"), Distance, *ActorLocation.ToString());
			continue;
		}

		// check if best
		if (Distance < BestDistance || BestDistance == 0)
		{
			BestTarget = Actor;
			BestDistance = Distance;
		}
	}

	return BestTarget;
}


