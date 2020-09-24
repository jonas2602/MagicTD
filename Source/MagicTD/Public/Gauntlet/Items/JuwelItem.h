// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gauntlet/Items/ItemBase.h"
#include "JuwelItem.generated.h"

class AJuwelBase;

/**
 * 
 */
UCLASS(Abstract)
class MAGICTD_API UJuwelItem : public UItemBase
{
	GENERATED_BODY()
	
public:
	UJuwelItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juwel")
	TSubclassOf<AJuwelBase> JuwelClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juwel")
	UStaticMesh* JuwelMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juwel")
	FLinearColor JuwelColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juwel")
	float IconScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juwel")
	float IconRotationAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Juwel")
	FVector2D IconAspectRatio;

};
