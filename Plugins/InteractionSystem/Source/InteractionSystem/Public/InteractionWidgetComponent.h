// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UInteractionWidgetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	FVector DefaultWidgetOffset = FVector(0.0f, 0.0f, 100.0f);
	FVector2D DefaultWidgetSize = FVector2D(100.0f, 135.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	FText WidgetDescription = FText::FromString("Interact");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	UTexture2D* WidgetImage;
};
