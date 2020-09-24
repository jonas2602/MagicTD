// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "PickupBase.generated.h"

//class UMeshComponent;

/**
 *
 */
UCLASS()
class INTERACTIONSYSTEM_API APickupBase : public AInteractableBase
{
	GENERATED_BODY()

	// Static mesh to represent the pickup in the level
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	UMeshComponent* PickupMesh;*/

public:
	// Sets default values for this actor's properties
	APickupBase();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Default")
	UMeshComponent* GetMeshComponent();
	virtual UMeshComponent* GetMeshComponent_Implementation() { return nullptr; }

protected:
	/************ InteractableBase Start *************/
	UFUNCTION(BlueprintInternalUseOnly)
	virtual bool InteractExec_Implementation(AActor* Interactor) final;

	UFUNCTION(BlueprintInternalUseOnly)
	virtual bool ReleaseExec_Implementation() final;
	/************* InteractableBase End **************/
	
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable|Pickup")
	bool OnPickup(AActor* Interactor);
	virtual bool OnPickup_Implementation(AActor* Interactor) { return true; }

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable|Pickup")
	bool OnDrop();
	virtual bool OnDrop_Implementation() { return true; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interactable|Pickup")
	EAttachmentRule AttachmentRule = EAttachmentRule::KeepWorld;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interactable|Pickup")
	FTransform RelativeTransform;

};
