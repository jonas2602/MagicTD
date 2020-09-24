// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "JuwelBase.generated.h"

class UStaticMeshComponent;
class UJuwelSocket;
class UParticleSystem;
class UParticleSystemComponent;
class UCurveFloat;
class UJuwelItem;

UENUM(BlueprintType)
enum class EJuwelState : uint8
{
	Free,
	Attached,
	Locked
};

UCLASS()
class MAGICTD_API AJuwelBase : public APickupBase
{
	GENERATED_BODY()
	
	/** Root Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* JuwelMesh;

public:	
	// Sets default values for this actor's properties
	AJuwelBase();
	FPrimaryAssetType GetItemType();
	UJuwelItem* GetJuwelItem() { return JuwelItem; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void Init(UJuwelItem* Template);
	virtual void Init_Implementation(UJuwelItem* Template) { }

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Default")
	static AJuwelBase* CreateJuwel(UObject* WorldContextObject, UJuwelItem* Template);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Default")
	static AJuwelBase* CreateJuwelAttached(UObject* WorldContextObject, UJuwelItem* Template, UJuwelSocket* Socket);

	virtual UMeshComponent* GetMeshComponent_Implementation() override { return JuwelMesh; }
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Default")
	virtual void ActivatePull();

	UFUNCTION(BlueprintCallable, Category = "Default")
	virtual void DeactivatePull();	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Default")
	virtual void ToggleLocked(bool Locked);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default")
	void ToggleGlow(bool Glow);
	virtual void ToggleGlow_Implementation(bool Glow) { }

protected:
	virtual bool OnPickup_Implementation(AActor* Interactor) override;
	virtual bool OnDrop_Implementation() override;
	virtual bool CanInteractExec_Implementation(AActor* Interactor) override;

private:
	UFUNCTION()
	void OnJuwelMeshBeginOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnJuwelMeshEndOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UFUNCTION(BlueprintCallable, Category = "Default")
	void UpdateClosestSocket();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void UpdateParticles();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void LerpToTarget(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Default")
	UJuwelSocket* FindClosestSocket();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	TArray<UJuwelSocket*> OverlappingSockets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	UJuwelSocket* ClosestSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	UParticleSystemComponent* PullEmitter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	bool SearchSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UJuwelItem* JuwelItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	EJuwelState JuwelState;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	UParticleSystem* PullEmitterTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	UCurveFloat* LerpCurve;
};
