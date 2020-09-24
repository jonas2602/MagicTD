// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GauntletTypes.h"
#include "Gauntlet.generated.h"

class AJuwelBase;
class UJuwelSocket;
class UItemBase;


UCLASS()
class MAGICTD_API AGauntlet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGauntlet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Default")
	void StartEditing();
	
	UFUNCTION(BlueprintCallable, Category = "Default")
	void StopEditing();

protected:
	UFUNCTION()
	void OnJuwelAttached(UJuwelSocket* Socket, AJuwelBase* Juwel);

	UFUNCTION()
	void OnJuwelDettached(UJuwelSocket* Socket, AJuwelBase* Juwel);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FGauntletConfig GauntletConfig;
};
