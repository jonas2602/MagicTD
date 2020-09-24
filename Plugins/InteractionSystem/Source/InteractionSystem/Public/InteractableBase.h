// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractionSystem.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InteractableBase.generated.h"

class UBillboardComponent;
class UInteractionWidgetComponent;
class UInteractionSocketComponent;
class UMeshComponent;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionStarted, AInteractableBase*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionFinished, AInteractableBase*, Interactable);


UENUM()
enum class EOutlineColor : uint8
{
	White = 255,
	Red = 126,
	Blue = 125,
	Green = 124,
};

UCLASS()
class INTERACTIONSYSTEM_API AInteractableBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UInteractionWidgetComponent* InteractableWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UInteractionSocketComponent* InteractableSocket;

public:
	// Sets default values for this actor's properties
	AInteractableBase();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Interactable")
	FInteractionStarted OnInteractionStarted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Interactable")
	FInteractionFinished OnInteractionFinished;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/************ Interaction Interface Start *************/
public:
	virtual bool Interact(AActor* Interactor) override;		// Please override InteractExec on children instead
	virtual	bool Release(AActor* Interactor) override;		// Please override ReleaseExec on children instead
	virtual bool CanInteract(AActor* Interactor) override;	// Please override CanInteractExec on children instead

	virtual void ToggleHighlight(bool Highlight) override;
	virtual	FVector GetInteractionLocation() override;
	virtual float GetInteractionMaxDistance() override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable", meta = (DisplayName = "Interact"))
	bool InteractExec(AActor* Interactor);
	virtual bool InteractExec_Implementation(AActor* Interactor) { return true; }

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable", meta = (DisplayName = "Release"))
	bool ReleaseExec();
	virtual bool ReleaseExec_Implementation() { return true; }

	UFUNCTION(BlueprintNativeEvent, Category = "Interactable", meta = (DisplayName = "CanInteract"))
	bool CanInteractExec(AActor* Interactor);
	virtual bool CanInteractExec_Implementation(AActor* Interactor) { return true; }

	// Returnes all meshes that should be outlined (Default => All)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void GetOutlineMeshes(TArray<UMeshComponent*>& OutComponents);
	virtual void GetOutlineMeshes_Implementation(TArray<UMeshComponent*>& OutComponents);

	UFUNCTION(BlueprintPure, Category = "Interactable")
	bool InUse() { return ActiveInteractor != nullptr; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	EOutlineColor OutlineColor = EOutlineColor::White;

	// Maximum Distance Interaction is allowed (0 = infinite) 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	float InteractionMaxDistance = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	AActor* ActiveInteractor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool InteractorSwitchable = false;
	/************* Interaction Interface End **************/

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	void InteractionTick(float DeltaTime);
	virtual void InteractionTick_Implementation(float DeltaTime) { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
	bool InteractionTickEnabled = true;

protected:
	UFUNCTION(BlueprintCallable, Category = "Default")
	void PlayMontageOnInteractor(UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable, Category = "Default")
	void StopMontageOnInteractor(UAnimMontage* Montage);

	UFUNCTION(BlueprintNativeEvent, Category = "Default")
	void OnMontageBlendedIn(UAnimMontage* Montage);
	virtual void OnMontageBlendedIn_Implementation(UAnimMontage* Montage) { }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	UAnimMontage* InteractorMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool AutoHandleMontage = true;

	// bool StartInteractionOnMontageBlendedIn = false;
};
