// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "ObjectiveTargetPoint.generated.h"

class AActor;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGICTD_API UObjectiveTargetPoint : public UBillboardComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UObjectiveTargetPoint();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default") // TODO: Change to Reserved Actor
	AActor* User;

	UFUNCTION(BlueprintCallable, Category = "Default", meta = (CallInEditor = "true"))
	bool TryProjectToNavigation(bool UseCurrentLocation = false, FVector Extends = FVector::ZeroVector);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FVector InitialRelativeLocation;
	
};
