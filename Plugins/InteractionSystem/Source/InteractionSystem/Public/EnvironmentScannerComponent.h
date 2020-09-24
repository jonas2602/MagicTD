// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractionSystem.h"
#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractableInterface.h"
#include "EnvironmentScannerComponent.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetChanged, AActor*, Target);


UCLASS(Blueprintable, abstract)
class INTERACTIONSYSTEM_API UEnvironmentScannerComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UEnvironmentScannerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Processing")
	FTargetChanged OnTargetChanged;

public:
	//UFUNCTION(BlueprintCallable, Category = "Scanning")
	virtual void EnableScanner();
	
	UFUNCTION(BlueprintCallable, Category = "Scanning")
	virtual void DisableScanner();

	UFUNCTION(BlueprintPure, Category = "Scanning")
	AActor* GetCurrentTarget() { return CurrentTarget; }

	UFUNCTION(BlueprintPure, Category = "Scanning")
	bool HasTarget() { return CurrentTarget != nullptr; }

	// TODO: IsOverlappingActor(AActor*) check, if actor is overlapping independent of is interactable or not?
	//		 -> NOT NESECCARY BECAUSE SPHERE ALREADY IMPLEMENTS IT

	// TODO: Listen for specific Actor?

protected:
	UFUNCTION(BlueprintCallable, Category = "Scanning")
	void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Scanning")
	void OnComponentEndOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
	UPROPERTY(BlueprintReadWrite, Category = "Scanning")
	FTimerHandle UpdateTargetHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Scanning")
	TArray<AActor*> NearInteractables;

protected:
	// Returns Target if InteractionTarget, returns nullptr if CombatTarget or no target
	UFUNCTION(BlueprintCallable, Category = "Processing")
	void UpdateTarget();

	// TODO: Make BlueprintNativeEvent
	UFUNCTION(BlueprintCallable, Category = "Processing")
	virtual AActor* CalcBestTarget(TArray<AActor*> Actors);

	UFUNCTION(BlueprintCallable, Category = "Processing")
	virtual bool AttemptInteraction(AActor*& OutInteractable); // TODO: only return Interactable if longer interaction follows (usable, pickup)

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Processing")
	bool AutoUpdate = true;

	// Rate to recalcutate the target
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Processing")
	float UpdateRate = 0.2f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Processing")
	AActor* CurrentTarget;

};
