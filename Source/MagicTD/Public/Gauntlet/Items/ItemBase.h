// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemBase.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class MAGICTD_API UItemBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	UItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemIcon;

	/** Type of this item, set in native parent class */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	FPrimaryAssetType ItemType;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
};
