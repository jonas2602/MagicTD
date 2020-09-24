// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "DataProviders/AIDataProvider.h"


#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "UObject/UnrealType.h"
#include "EnvironmentQuery/EnvQueryContext.h"

#include "EnvQueryGenerator_TargetBorder.generated.h"

/**
 *
 */

//USTRUCT()
//struct MAGICTD_API FAIDataProviderObjectValue : public FAIDataProviderTypedValue
//{
//	GENERATED_USTRUCT_BODY()
//
//	FAIDataProviderObjectValue()
//	{
//		PropertyType = UObjectProperty::StaticClass();
//	}
//
//	UObject* GetValue() const
//	{
//		return GetRawValuePtr<UObject>();
//	}
//
//	virtual FString ValueToString() const override
//	{
//		return "My Default Value";
//	}
//};

UCLASS(meta = (DisplayName = "Target Border"))
class MAGICTD_API UEnvQueryGenerator_TargetBorder : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = Generator)
	TSubclassOf<UEnvQueryContext> GenerateAround;

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
