// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "JuwelComponentBase.generated.h"

class UJuwelItem;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGICTD_API UJuwelComponentBase : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UJuwelComponentBase();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void Init(UJuwelItem* Template);

	UFUNCTION(BlueprintCallable, Category = "Default")
	void ToggleGlow(bool Glow);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UJuwelItem* JuwelItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	UMaterialInterface* JuwelMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	float GlowIntensity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	UMaterialInstanceDynamic* MaterialInstance;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName JuwelColorMIP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName JuwelIntensityMIP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName JuwelIconMIP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName IconScaleMIP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName IconRotationMIP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName IconAspectRatioMIP;
};
