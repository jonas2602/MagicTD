// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEnvQueryBlueprintWrapper.h"
#include "EnvQueryItemType_SceneComponent.h"
#include "Components/SceneComponent.h"


TArray<USceneComponent*> UMyEnvQueryBlueprintWrapper::GetResultsAsComponents()
{
	TArray<USceneComponent*> Results;

	if (QueryResult.IsValid() && ItemType->IsChildOf(UEnvQueryItemType_SceneComponent::StaticClass()))
	{
		if (RunMode != EEnvQueryRunMode::AllMatching)
		{
			Results.Add(GetItemAsComponent(0));
		}
		else
		{
			GetAllAsComponents(Results);
		}
	}

	return Results;
}

USceneComponent* UMyEnvQueryBlueprintWrapper::GetItemAsComponent(int32 Index) const
{
	if (QueryResult->Items.IsValidIndex(Index) &&
		ItemType->IsChildOf(UEnvQueryItemType_SceneComponent::StaticClass()))
	{
		UEnvQueryItemType_SceneComponent* DefTypeOb = ItemType->GetDefaultObject<UEnvQueryItemType_SceneComponent>();
		return DefTypeOb->GetComponent(QueryResult->RawData.GetData() + QueryResult->Items[Index].DataOffset);
	}

	return nullptr;
}

void UMyEnvQueryBlueprintWrapper::GetAllAsComponents(TArray<USceneComponent*>& OutActors) const
{
	if (ItemType->IsChildOf(UEnvQueryItemType_SceneComponent::StaticClass()) && QueryResult->Items.Num() > 0)
	{
		UEnvQueryItemType_SceneComponent* DefTypeOb = ItemType->GetDefaultObject<UEnvQueryItemType_SceneComponent>();

		OutActors.Reserve(OutActors.Num() + QueryResult->Items.Num());

		for (const FEnvQueryItem& Item : QueryResult->Items)
		{
			OutActors.Add(DefTypeOb->GetComponent(QueryResult->RawData.GetData() + Item.DataOffset));
		}
	}
}