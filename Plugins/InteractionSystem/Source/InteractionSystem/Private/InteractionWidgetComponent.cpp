// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionWidgetComponent.h"
#include "InteractionSystem.h"
#include "InteractionWidget.h"


// Sets default values for this component's properties
UInteractionWidgetComponent::UInteractionWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetWidgetClass(UInteractionWidget::StaticClass());
	SetWidgetSpace(EWidgetSpace::Screen);
	SetRelativeLocation(DefaultWidgetOffset);
	SetDrawSize(DefaultWidgetSize);
}


// Called when the game starts
void UInteractionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UInteractionWidget* Widget = Cast<UInteractionWidget>(GetUserWidgetObject());
	if (Widget == nullptr)
	{
		UE_LOG(LogInteraction, Error, TEXT("Widget not Valid"));
		return;
	}
	
	Widget->SetInteractionText(WidgetDescription);
	Widget->SetInteractionImage(WidgetImage);
	
}


// Called every frame
void UInteractionWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

