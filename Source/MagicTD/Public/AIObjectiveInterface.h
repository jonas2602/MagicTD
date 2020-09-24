// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIObjectiveInterface.generated.h"

class AActor;
class UObjectiveTargetPoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectiveCompletedDelegate, AActor*, AIObjective);


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UAIObjectiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAGICTD_API IAIObjectiveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "AI")
	FObjectiveCompletedDelegate OnCompleted;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI", meta = (CallInEditor = "true"))
	float GetHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI", meta = (CallInEditor = "true"))
	float GetPriority();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI", meta = (CallInEditor = "true"))
	TArray<UObjectiveTargetPoint*> GetFreeAISpots();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI", meta = (CallInEditor = "true"))
	TArray<UObjectiveTargetPoint*> GetAllAISpots();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI", meta = (CallInEditor = "true"))
	bool RequestSpot(UObjectiveTargetPoint*& OutSpot, AActor* Pawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI", meta = (CallInEditor = "true"))
	bool FreeSpot(UObjectiveTargetPoint* Spot);
};
