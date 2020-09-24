// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectiveTargetPoint.h"
#include "Engine/World.h"
#include "NavigationSystem.h"

// Sets default values for this component's properties
UObjectiveTargetPoint::UObjectiveTargetPoint()
	:User(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UObjectiveTargetPoint::BeginPlay()
{
	Super::BeginPlay();
}

bool UObjectiveTargetPoint::TryProjectToNavigation(bool UseCurrentLocation, FVector Extends)
{
	if (InitialRelativeLocation == FVector::ZeroVector)
	{
		// PreSet Initial Location if not already set
		InitialRelativeLocation = GetRelativeTransform().GetLocation();
	}

	// Get Initial Location in WorldSpace
	FVector InitialAbsoluteLocation = GetAttachParent()->GetComponentTransform().TransformPosition(InitialRelativeLocation);
	FVector UsedLocation = UseCurrentLocation ? GetComponentLocation() : InitialAbsoluteLocation;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation OutNavLocation;
	if (NavSys->ProjectPointToNavigation(UsedLocation, OutNavLocation, Extends))
	{
		SetWorldLocation(OutNavLocation.Location);
		return true;
	}

	return false;
}