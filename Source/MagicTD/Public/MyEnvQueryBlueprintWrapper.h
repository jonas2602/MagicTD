// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "MyEnvQueryBlueprintWrapper.generated.h"

class USceneComponent;

/**
 * 
 */
UCLASS()
class MAGICTD_API UMyEnvQueryBlueprintWrapper : public UEnvQueryInstanceBlueprintWrapper
{
	GENERATED_BODY()
	
public:
	/** return an array filled with resulting Components. Note that it makes sense only if ItemType is a EnvQueryItemType_SceneComponent-derived type */
	UFUNCTION(BlueprintPure, Category = "AI|EQS")
	TArray<USceneComponent*> GetResultsAsComponents();
	
protected:
	/** item accessors for basic types */
	USceneComponent* GetItemAsComponent(int32 Index) const;

	/** note that this function does not strip out the null-actors to not mess up results of GetItemScore(Index) calls*/
	void GetAllAsComponents(TArray<USceneComponent*>& OutComponents) const;
	
};
