// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvironmentScannerComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UEnvironmentScannerComponent::UEnvironmentScannerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	// SetSphereRadius(DefaultRadius);
}


// Called when the game starts
void UEnvironmentScannerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	
	// Add initialy overlapping Actors
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	OverlappingActors.Remove(GetOwner()); // ignore self

	for (AActor* Actor : OverlappingActors)
	{
		// Can interact with?
		if (Actor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			NearInteractables.AddUnique(Actor);
		}
	}
}


// Called every frame
void UEnvironmentScannerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnvironmentScannerComponent::EnableScanner()
{
	// Bind Begin/End Overlap Functions
	OnComponentBeginOverlap.AddDynamic(this, &UEnvironmentScannerComponent::OnComponentBeginOverlapHandle);
	OnComponentEndOverlap.AddDynamic(this, &UEnvironmentScannerComponent::OnComponentEndOverlapHandle);

	// Add initialy overlapping Actors
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	OverlappingActors.Remove(GetOwner()); // ignore self

	for (AActor* Actor : OverlappingActors)
	{
		// Can interact with?
		if (Actor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			NearInteractables.AddUnique(Actor);
		}
	}

	// Start Target Processing
	if (AutoUpdate)
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateTargetHandle, this, &UEnvironmentScannerComponent::UpdateTarget, UpdateRate, true, UpdateRate);
	}

	UE_LOG(LogInteraction, Warning, TEXT("Scanner enabled"));
}

void UEnvironmentScannerComponent::DisableScanner()
{
	// Remove Begin/End Overlap Function Bindings
	OnComponentBeginOverlap.Clear();
	OnComponentEndOverlap.Clear();

	// Stop Target Processing
	GetWorld()->GetTimerManager().ClearTimer(UpdateTargetHandle);

	// Clear current Target
	if (CurrentTarget)
	{
		IInteractableInterface* TargetInterface = Cast<IInteractableInterface>(CurrentTarget);
		TargetInterface->ToggleHighlight(false);
		CurrentTarget = nullptr;
	}

	// Clear overlapping Targets
	NearInteractables.Reset();

	UE_LOG(LogInteraction, Warning, TEXT("Scanner disabled"));
}

void UEnvironmentScannerComponent::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Can interact with?
	if (OtherActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		// UE_LOG(LogInteraction, Log, TEXT("Component begins to overlap %s"), *OtherActor->GetName());
		NearInteractables.AddUnique(OtherActor);
	}
}

void UEnvironmentScannerComponent::OnComponentEndOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Can interact with?
	if (OtherActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		// UE_LOG(LogInteraction, Log, TEXT("Component ends to overlap %s"), *OtherActor->GetName());
		NearInteractables.Remove(OtherActor);
	}
}

void UEnvironmentScannerComponent::UpdateTarget()
{
	// Calculate new Target
	AActor* NewTarget = CalcBestTarget(NearInteractables);


	// Promote Update
	if (NewTarget != CurrentTarget)
	{
		if (CurrentTarget)
		{
			IInteractableInterface* TargetInterface = Cast<IInteractableInterface>(CurrentTarget);
			TargetInterface->ToggleHighlight(false);
		}

		if (NewTarget)
		{
			IInteractableInterface* TargetInterface = Cast<IInteractableInterface>(NewTarget);
			TargetInterface->ToggleHighlight(true);
		}

		CurrentTarget = NewTarget;
		OnTargetChanged.Broadcast(NewTarget);

		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Switched Target");
		UE_LOG(LogInteraction, Error, TEXT("Switched Target"));
	}

}

AActor* UEnvironmentScannerComponent::CalcBestTarget(TArray<AActor*> Actors)
{
	if (Actors.Num() > 0)
	{
		return Actors[0];
	}

	return nullptr;
}

bool UEnvironmentScannerComponent::AttemptInteraction(AActor*& OutInteractable)
{
	// Has Target?
	if (CurrentTarget == nullptr)
	{
		UE_LOG(LogInteraction, Warning, TEXT("No Target to interact with"));
		return false;
	}

	// Call Interact on current Target
	if (Cast<IInteractableInterface>(CurrentTarget)->Interact(GetOwner()))
	{
		OutInteractable = CurrentTarget;

		// make shure that no one can interact twice
		UpdateTarget();
		return true;
	}
	// TODO: Custom Interact function for each type?

	// TODO: Callback to Interactor to handle different Interaction Types (Pickups, Buffs, Usables, ...)
	// TODO: New Component PickupSocket to handle these stuff (hasItem, Animations, ...) without the need to put in in each class that will use it

	return false;
}