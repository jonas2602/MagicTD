// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionWidget.h"
#include "WidgetTree.h"
#include "Button.h"
#include "Widget.h"
#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"
#include "VerticalBox.h"
#include "Image.h"
#include "TextBlock.h"
#include "Engine/Texture2D.h"
#include "Engine/Engine.h"
#include "InteractionSystem.h"

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind delegates here.
	// UE_LOG(LogInteraction, Error, TEXT("native construct"));
}

TSharedRef<SWidget> UInteractionWidget::RebuildWidget()
{
	// UE_LOG(LogInteraction, Error, TEXT("Start Rebuild"));

	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
	if (!RootWidget)
	{
		RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootWidget"));

		UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(RootWidget->Slot);
		if (RootWidgetSlot)
		{
			RootWidgetSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
			RootWidgetSlot->SetOffsets(FMargin(0.0f, 0.0f));
		}

		// If GetRootWidget() is still null
		WidgetTree->RootWidget = RootWidget;
	}

	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("RootWidget exists? %s"), RootWidget ? TEXT("true") : TEXT("false")));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("WidgetTree exists? %s"), WidgetTree ? TEXT("true") : TEXT("false")));

	if (!RootWidget || !WidgetTree)
	{
		UE_LOG(LogInteraction, Error, TEXT("RootWidget or WidgetTree invalid"));
		return Widget;
	}
	
	// Create Vertical Box
	UVerticalBox* VerticalBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VerticalBox"));
	UCanvasPanelSlot* VerticalBoxSlot = Cast<UCanvasPanelSlot>(VerticalBox->Slot);
	if (VerticalBoxSlot)
	{
		VerticalBoxSlot->SetAutoSize(true);
	}
	RootWidget->AddChild(VerticalBox);


	// Create Image
	// UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ImagePath));
	InteractionImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("InteractionImage"));
	// Image->SetVisibility(ESlateVisibility::Visible);
	InteractionImage->SetBrushFromTexture(InteractionImageValue);
	InteractionImage->Brush.ImageSize = FVector2D(100.0f, 100.0f);
	VerticalBox->AddChild(InteractionImage);


	// Create Text
	InteractionText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("InteractionText"));
	InteractionText->SetText(InteractionTextValue);
	InteractionText->Font.Size = 20;
	InteractionText->SetJustification(ETextJustify::Center);
	VerticalBox->AddChild(InteractionText);


	// ExampleButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("MyButtonName")); // The second parameter is the name and is optional.
	// ExampleButton->OnClicked.AddDynamic(this, &MyUserWidget::OnButtonClicked); // Example click binding.
	// RootWidget->AddChild(ExampleButton);
	// UCanvasPanelSlot* ButtonSlot = Cast<UCanvasPanelSlot>(ExampleButton->Slot);
	// if (ButtonSlot)
	// {
	//		ButtonSlot->SetAutoSize(true);
	// }
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Example Button created")));
	
	// UE_LOG(LogInteraction, Error, TEXT("Rebuild finished"));
	return Widget;
}

void UInteractionWidget::SetInteractionText(FText NewText)
{
	// Set value that will be used, when widget is rebuild
	InteractionTextValue = NewText;

	// If widget already build
	if (InteractionText != nullptr)
	{
		// Update widget
		InteractionText->SetText(NewText);
	}

}

void UInteractionWidget::SetInteractionImage(UTexture2D* NewImage)
{
	// Set value that will be used, when widget is rebuild
	InteractionImageValue = NewImage;

	// If widget already build
	if (InteractionImage != nullptr)
	{
		// Update widget
		InteractionImage->SetBrushFromTexture(NewImage);
	}

}
