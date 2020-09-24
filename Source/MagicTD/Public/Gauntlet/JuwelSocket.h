// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GauntletTypes.h"
#include "JuwelSocket.generated.h"

class AJuwelBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJuwelAttached, UJuwelSocket*, Socket, AJuwelBase*, Juwel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJuwelDettached, UJuwelSocket*, Socket, AJuwelBase*, Juwel);

/**
 *
 */

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGICTD_API UJuwelSocket : public USphereComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Processing")
	FJuwelAttached OnJuwelAttached;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Processing")
	FJuwelDettached OnJuwelDettached;

	ESocketType GetSocketType() { return SocketType; }
	FPrimaryAssetType GetJuwelType() { return JuwelType; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Default")
	bool TryAttachJuwel(AJuwelBase* Juwel);

	UFUNCTION(BlueprintCallable, Category = "Default")
	void ToggleLocked(bool Locked);

	UFUNCTION()
	void OnJuwelGrabbed(AInteractableBase* Interactable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	bool IsJuwelAttachable(AJuwelBase* Juwel);
	virtual bool IsJuwelAttachable_Implementation(AJuwelBase* Juwel);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	bool Locked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FPrimaryAssetType JuwelType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	ESocketType SocketType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	AJuwelBase* AttachedJuwel;
};
