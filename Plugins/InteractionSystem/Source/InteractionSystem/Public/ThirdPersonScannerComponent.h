// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentScannerComponent.h"
#include "ThirdPersonScannerComponent.generated.h"

/**
 * 
 */

class UCameraComponent;

UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UThirdPersonScannerComponent : public UEnvironmentScannerComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Scanning")
	void EnableScanner(UCameraComponent* Camera);
	virtual void EnableScanner() override;
	
protected:
	virtual AActor* CalcBestTarget(TArray<AActor*> Actors) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Processing")
	UCameraComponent* OwnerCamera;
	
	// 0.0f means no threshold
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Processing")
	float InteractionDotThreshold = 0.85f;
};
