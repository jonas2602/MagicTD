// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentScannerComponent.h"
#include "MotionControllerScannerComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UMotionControllerScannerComponent : public UEnvironmentScannerComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Scanning")
	virtual void EnableScanner() override;

protected:
	virtual AActor* CalcBestTarget(TArray<AActor*> Actors) override;

	// 0.0f means no threshold
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Processing")
	float InteractionDistanceThreshold = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scanning")
	bool StartWithScannerEnabled = true;

};
