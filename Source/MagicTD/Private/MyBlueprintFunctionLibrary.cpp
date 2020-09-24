// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlueprintFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

float UMyBlueprintFunctionLibrary::GetClosestPointOnActorCollision(const FVector& Point, AActor* Actor, FVector& OutPointOnBody)
{
	float ClosestDistance = -1;

	for(UActorComponent* Component : Actor->GetComponentsByClass(UPrimitiveComponent::StaticClass()))
	{
		FVector OutPoint;
		float Distance = Cast<UPrimitiveComponent>(Component)->GetClosestPointOnCollision(Point, OutPoint);
		if (Distance < 0)
		{
			// Distances < 0 are not valid (from non boundingbox components)
			continue;
		}

		// UE_LOG(LogClass, Error, TEXT("%s is %f away"), *Component->GetName(), Distance);

		if (Distance < ClosestDistance || ClosestDistance == -1)
		{
			ClosestDistance = Distance;
			OutPointOnBody = OutPoint;
		}
	}

	return ClosestDistance;
}

