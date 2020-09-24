// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryGenerator_TargetBorder.h"
#include "AIObjectiveInterface.h"
#include "ObjectiveTargetPoint.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvQueryItemType_SceneComponent.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryGenerator_TargetBorder::UEnvQueryGenerator_TargetBorder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ItemType = UEnvQueryItemType_SceneComponent::StaticClass();

	GenerateAround = UEnvQueryContext_Querier::StaticClass();
}

void UEnvQueryGenerator_TargetBorder::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	UObject* BindOwner = QueryInstance.Owner.Get();
	//	/*
	//	const int32 ItemCount = FPlatformMath::TruncToInt((RadiusValue * 2.0f / DensityValue) + 1);
	//	const int32 ItemCountHalf = ItemCount / 2;
	//
	TArray<AActor*> ContextActors;
	QueryInstance.PrepareContext(GenerateAround, ContextActors);

	// Only allow a single Target;
	if (!ContextActors.IsValidIndex(0))
	{
		UE_LOG(LogClass, Error, TEXT("Context result empty"));
		return;
	}
	AActor* TargetActor = ContextActors[0];

	// Get Target Points
	if (!TargetActor->Implements<UAIObjectiveInterface>())
	{
		UE_LOG(LogClass, Error, TEXT("Context result not implementing AIObjectiveInterface"));
		return;
	}
	TArray<UObjectiveTargetPoint*> TargetPoints = IAIObjectiveInterface::Execute_GetFreeAISpots(TargetActor);
	QueryInstance.AddItemData<UEnvQueryItemType_SceneComponent>(TargetPoints);
	// UE_LOG(LogClass, Warning, TEXT("Found %d Points at %s"), TargetPoints.Num(), *TargetActor->GetName());

	/*TArray<FNavLocation> GridPoints;
	GridPoints.Reserve(TargetPoints.Num());
	for (UObjectiveTargetPoint* Point : TargetPoints)
	{
		const FNavLocation TestPoint = FNavLocation(Point->GetComponentLocation());
		GridPoints.Add(TestPoint);
	}

	ProjectAndFilterNavPoints(GridPoints, QueryInstance);
	StoreNavPoints(GridPoints, QueryInstance);*/
}

FText UEnvQueryGenerator_TargetBorder::GetDescriptionTitle() const
{
	return FText::Format(LOCTEXT("BorderDescriptionGenerateAroundContext", "{0}: generate around {1}"),
		Super::GetDescriptionTitle(), UEnvQueryTypes::DescribeContext(GenerateAround));
}

FText UEnvQueryGenerator_TargetBorder::GetDescriptionDetails() const
{
	FText Desc = FText::FromName("");

	FText ProjDesc = ProjectionData.ToText(FEnvTraceData::Brief);
	if (!ProjDesc.IsEmpty())
	{
		FFormatNamedArguments ProjArgs;
		ProjArgs.Add(TEXT("Description"), Desc);
		ProjArgs.Add(TEXT("ProjectionDescription"), ProjDesc);
		Desc = FText::Format(LOCTEXT("SimpleGridDescriptionWithProjection", "{Description}, {ProjectionDescription}"), ProjArgs);
	}

	return Desc;
}

#undef LOCTEXT_NAMESPACE
