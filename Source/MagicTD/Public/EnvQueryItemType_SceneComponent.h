// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvQueryItemType_SceneComponent.generated.h"

class USceneComponent;
class UBlackboardComponent;
struct FBlackboardKeySelector;

struct FEnvQueryContextData;
struct FWeakObjectPtr;

/**
 *
 */
UCLASS()
class MAGICTD_API UEnvQueryItemType_SceneComponent : public UEnvQueryItemType_VectorBase
{
	GENERATED_BODY()

public:
	typedef const FWeakObjectPtr& FValueType;

	UEnvQueryItemType_SceneComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static USceneComponent* GetValue(const uint8* RawData);
	static void SetValue(uint8* RawData, const FWeakObjectPtr& Value);

	static void SetContextHelper(FEnvQueryContextData& ContextData, const USceneComponent* SingleComponent);
	static void SetContextHelper(FEnvQueryContextData& ContextData, const TArray<const USceneComponent*>& MultipleComponents);
	static void SetContextHelper(FEnvQueryContextData& ContextData, const TArray<USceneComponent*>& MultipleComponents);

	virtual FVector GetItemLocation(const uint8* RawData) const override;
	virtual FRotator GetItemRotation(const uint8* RawData) const override;
	virtual USceneComponent* GetComponent(const uint8* RawData) const;

	virtual void AddBlackboardFilters(FBlackboardKeySelector& KeySelector, UObject* FilterOwner) const override;
	virtual bool StoreInBlackboard(FBlackboardKeySelector& KeySelector, UBlackboardComponent* Blackboard, const uint8* RawData) const override;
	virtual FString GetDescription(const uint8* RawData) const override;

};
