// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvQueryItemType_SceneComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SceneComponent.h"

#include "UObject/WeakObjectPtr.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

UEnvQueryItemType_SceneComponent::UEnvQueryItemType_SceneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ValueSize = sizeof(FWeakObjectPtr);
}

void UEnvQueryItemType_SceneComponent::AddBlackboardFilters(FBlackboardKeySelector& KeySelector, UObject* FilterOwner) const
{
	Super::AddBlackboardFilters(KeySelector, FilterOwner);
	KeySelector.AddObjectFilter(FilterOwner, GetClass()->GetFName(), AActor::StaticClass());
}

bool UEnvQueryItemType_SceneComponent::StoreInBlackboard(FBlackboardKeySelector& KeySelector, UBlackboardComponent* Blackboard, const uint8* RawData) const
{
	bool bStored = Super::StoreInBlackboard(KeySelector, Blackboard, RawData);
	if (!bStored && KeySelector.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* MyObject = GetComponent(RawData);
		Blackboard->SetValue<UBlackboardKeyType_Object>(KeySelector.GetSelectedKeyID(), MyObject);

		bStored = true;
	}

	return bStored;
}

FString UEnvQueryItemType_SceneComponent::GetDescription(const uint8* RawData) const
{
	const USceneComponent* Component = GetComponent(RawData);
	return GetNameSafe(Component);
}

USceneComponent* UEnvQueryItemType_SceneComponent::GetComponent(const uint8* RawData) const
{
	return UEnvQueryItemType_SceneComponent::GetValue(RawData);
}



USceneComponent* UEnvQueryItemType_SceneComponent::GetValue(const uint8* RawData)
{
	FWeakObjectPtr WeakObjPtr = GetValueFromMemory<FWeakObjectPtr>(RawData);
	return (USceneComponent*)(WeakObjPtr.Get());
}

void UEnvQueryItemType_SceneComponent::SetValue(uint8* RawData, const FWeakObjectPtr& Value)
{
	FWeakObjectPtr WeakObjPtr(Value);
	SetValueInMemory<FWeakObjectPtr>(RawData, WeakObjPtr);
}

FVector UEnvQueryItemType_SceneComponent::GetItemLocation(const uint8* RawData) const
{
	USceneComponent* MyComponent = UEnvQueryItemType_SceneComponent::GetValue(RawData);
	return MyComponent ? MyComponent->GetComponentLocation() : FVector::ZeroVector;
}

FRotator UEnvQueryItemType_SceneComponent::GetItemRotation(const uint8* RawData) const
{
	USceneComponent* MyComponent = UEnvQueryItemType_SceneComponent::GetValue(RawData);
	return MyComponent ? MyComponent-> GetComponentRotation() : FRotator::ZeroRotator;
}

void UEnvQueryItemType_SceneComponent::SetContextHelper(FEnvQueryContextData& ContextData, const USceneComponent* SingleComponent)
{
	ContextData.ValueType = UEnvQueryItemType_SceneComponent::StaticClass();
	ContextData.NumValues = 1;
	ContextData.RawData.SetNumUninitialized(sizeof(FWeakObjectPtr));

	UEnvQueryItemType_SceneComponent::SetValue(ContextData.RawData.GetData(), SingleComponent);
}

void UEnvQueryItemType_SceneComponent::SetContextHelper(FEnvQueryContextData& ContextData, const TArray<const USceneComponent*>& MultipleComponents)
{
	ContextData.ValueType = UEnvQueryItemType_SceneComponent::StaticClass();
	ContextData.NumValues = MultipleComponents.Num();
	ContextData.RawData.SetNumUninitialized(sizeof(FWeakObjectPtr) * MultipleComponents.Num());

	uint8* RawData = (uint8*)ContextData.RawData.GetData();
	for (int32 ActorIndex = 0; ActorIndex < MultipleComponents.Num(); ActorIndex++)
	{
		UEnvQueryItemType_SceneComponent::SetValue(RawData, MultipleComponents[ActorIndex]);
		RawData += sizeof(FWeakObjectPtr);
	}
}

void UEnvQueryItemType_SceneComponent::SetContextHelper(FEnvQueryContextData& ContextData, const TArray<USceneComponent*>& MultipleComponents)
{
	ContextData.ValueType = UEnvQueryItemType_SceneComponent::StaticClass();
	ContextData.NumValues = MultipleComponents.Num();
	ContextData.RawData.SetNumUninitialized(sizeof(FWeakObjectPtr)* MultipleComponents.Num());

	uint8* RawData = (uint8*)ContextData.RawData.GetData();
	for (int32 ComponentIndex = 0; ComponentIndex < MultipleComponents.Num(); ComponentIndex++)
	{
		UEnvQueryItemType_SceneComponent::SetValue(RawData, MultipleComponents[ComponentIndex]);
		RawData += sizeof(FWeakObjectPtr);
	}
}
