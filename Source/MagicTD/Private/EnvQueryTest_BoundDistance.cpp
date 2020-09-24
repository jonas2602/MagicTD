// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTest_BoundDistance.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

#include "GameFramework/Actor.h"
#include "Components/MeshComponent.h"

UEnvQueryTest_BoundDistance::UEnvQueryTest_BoundDistance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DistanceTo = UEnvQueryContext_Querier::StaticClass();
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

void UEnvQueryTest_BoundDistance::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	// don't support context Item here, it doesn't make any sense
	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(DistanceTo, ContextLocations))
	{
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		AActor* ObjectiveActor = QueryInstance.GetItemAsActor(It.GetIndex());
		TArray<UActorComponent*> MeshComponents = ObjectiveActor->GetComponentsByClass(UMeshComponent::StaticClass());

		//UE_LOG(LogClass, Error, TEXT("Name: %s"), *ObjectiveActor->GetName());
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
		{
			float BestDistance = -1;

			for (UActorComponent* Mesh : MeshComponents)
			{
				FVector PointOnCollision;
				float Distance = Cast<UMeshComponent>(Mesh)->GetDistanceToCollision(ContextLocations[ContextIndex], PointOnCollision);
				if (Distance < BestDistance || BestDistance == -1)
				{
					//UE_LOG(LogClass, Error, TEXT("Distance: %f"), Distance);
					BestDistance = Distance;
				}
			}

			It.SetScore(TestPurpose, FilterType, 1, MinThresholdValue, MaxThresholdValue);
		}
	}

	/*for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
		{
			const float Distance = CalcDistance3D(ItemLocation, ContextLocations[ContextIndex]);
			It.SetScore(TestPurpose, FilterType, Distance, MinThresholdValue, MaxThresholdValue);
		}
	}*/
}

FText UEnvQueryTest_BoundDistance::GetDescriptionTitle() const
{
	return FText::FromString("My Description Title");
}

FText UEnvQueryTest_BoundDistance::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}

