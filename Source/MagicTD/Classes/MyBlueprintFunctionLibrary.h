// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

class AActor;

/**
 * 
 */
UCLASS()
class MAGICTD_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Collision")
	static float GetClosestPointOnActorCollision(const FVector& Point, AActor* Actor, FVector& OutPointOnBody);
	
	
};
