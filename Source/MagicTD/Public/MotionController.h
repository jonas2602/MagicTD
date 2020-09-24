// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellSystemInterface.h"
#include "GauntletTypes.h"
#include "MotionController.generated.h"

class USceneComponent;
class USkillBase;
class USkeletalMeshComponent;
class UMotionControllerComponent;
class USpellSystemComponent;
class UJuwelComponentBase;
class UMySaveGame;

UCLASS()
class MAGICTD_API AMyMotionController : public AActor, public ISpellSystemInterface
{
	GENERATED_BODY()

	/** Root Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSocket;

public:
	// Sets default values for this actor's properties
	AMyMotionController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Meta = (ExposeOnSpawn = true), Category = "Default")
	EControllerHand Hand = EControllerHand::Left;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Input")
	float GetGripAxis();
	virtual float GetGripAxis_Implementation() { return 0.0f; }

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Input")
	float GetTriggerAxis();
	virtual float GetTriggerAxis_Implementation() { return 0.0f; }

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Input")
	float GetThumbAxis();
	virtual float GetThumbAxis_Implementation() { return 0.0f; }

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Input")
	void GetStickAxes(float& AxisValueRight, float& AxisValueUp);
	virtual void GetStickAxes_Implementation(float& AxisValueRight, float& AxisValueUp) { }

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Input")
	FRotator GetStickRotation();
	virtual FRotator GetStickRotation_Implementation() { return FRotator(); }


	/************ SpellSystem Interface Start *************/
public:
	virtual USpellSystemComponent* GetSpellSystemComponent_Implementation() override { return nullptr; }
	virtual bool Overlapping_Implementation(AMyMotionController*& OverlappingController) override { return false; }
	virtual AGauntlet* GetGauntlet_Implementation() { return nullptr; }
	virtual bool GetJuwelsToSpell_Implementation(TSubclassOf<USkillBase> SpellClass, UJuwelComponentBase*& OutSpellJuwel, TArray<UJuwelComponentBase*>& OutModJuwels) { return false; }
	/************* SpellSystem Interface End **************/

protected:
	UFUNCTION(BlueprintCallable, Category = "Default")
	bool SaveGauntletConfig(FGauntletConfig GauntletConfig);

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool LoadGauntletConfig(FGauntletConfig& GauntletConfig);


	friend USkillBase;
};
