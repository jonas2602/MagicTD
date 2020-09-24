// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */

class UButton;
class UImage;
class UTextBlock;


UCLASS()
class INTERACTIONSYSTEM_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UFUNCTION(BlueprintCallable, Category = "InteractionWidget")
	void SetInteractionText(FText NewText);

	UFUNCTION(BlueprintCallable, Category = "InteractionWidget")
	void SetInteractionImage(UTexture2D* NewImage);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* InteractionImage;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UTextBlock* InteractionText;

	FText InteractionTextValue = FText::FromString("Interact");
	UTexture2D* InteractionImageValue;
};
