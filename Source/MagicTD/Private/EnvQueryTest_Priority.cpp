// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryTest_Priority.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"


void UEnvQueryTest_Priority::RunTest(FEnvQueryInstance& QueryInstance) const
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
	if (!QueryInstance.PrepareContext(UEnvQueryContext_Querier::StaticClass(), ContextLocations))
	{
		return;
	}
	
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
		{
			const float Distance = (ContextLocations[ContextIndex] - ItemLocation).Size();
			It.SetScore(TestPurpose, FilterType, Distance, MinThresholdValue, MaxThresholdValue);
		}
	}
}

FText UEnvQueryTest_Priority::GetDescriptionTitle() const
{
	return FText::FromString("My Description Title");
}

FText UEnvQueryTest_Priority::GetDescriptionDetails() const
{
	return FText::FromString("My Description Details");
}


