// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UInteractionSocketComponent;

/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API AWeaponBase : public APickupBase
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	UInteractionSocketComponent* InteractionSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* DamageCapsule;

public:
	// Sets default values for this actor's properties
	AWeaponBase();
	
protected:
	UFUNCTION(BlueprintInternalUseOnly)
	virtual UMeshComponent* GetMeshComponent_Implementation() override { return WeaponMesh; }
};
