// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Gauntlet/GauntletTypes.h"
#include "MotionControllerPawn.generated.h"

class USkillBase;

UCLASS()
class MAGICTD_API AMotionControllerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMotionControllerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool SaveGauntletConfig();

	UFUNCTION(BlueprintCallable, Category = "Default")
	bool LoadGauntletConfig();

	UFUNCTION(BlueprintCallable, Category = "Default")
	UActorComponent* AddActorComponent(TSubclassOf<UActorComponent> ComponentClass, FName Name);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	FGauntletConfig LeftGauntletConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	FGauntletConfig RightGauntletConfig;
};
