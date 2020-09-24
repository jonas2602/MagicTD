// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SkillBase.generated.h"

class UAnimInstance;
class UAnimMontage;
class AMyMotionController;
class AActor;
class AGauntlet;
class UJuwelComponentBase;
class UModificationItem;
class UWorld;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillFinishedDelegate, USkillBase*, Skill);


UENUM()
enum class EActionBlockType : uint8
{
	None,
	Hand,
	BothHands,
	ExceptSame
};

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGICTD_API USkillBase : public UObject
{
	GENERATED_BODY()


public:
	// Sets default values for this component's properties
	USkillBase();
	virtual UWorld* GetWorld() const override
	{
		if (HasAllFlags(RF_ClassDefaultObject))
		{
			// If we are a CDO, we must return nullptr instead of calling Outer->GetWorld() to fool UObject::ImplementsGetWorld.
			return nullptr;
		}
		return GetOuter()->GetWorld();
	}

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Skill")
	FSkillFinishedDelegate OnFinished;

protected:
	UFUNCTION(BlueprintPure, Category = "Skill")
	UAnimInstance* GetAnimBP(AMyMotionController* Controller);

	UFUNCTION(BlueprintPure, Category = "Skill")
	USceneComponent* GetProjectileSocket(AMyMotionController* Controller);

	UFUNCTION(BlueprintPure, Category = "Gauntlet")
	AGauntlet* GetGauntlet(AMyMotionController* Controller);

	UFUNCTION(BlueprintPure, Category = "Gauntlet")
	bool GetSkillJuwels(UJuwelComponentBase*& OutSpellJuwel, TArray<UJuwelComponentBase*>& OutModJuwels);

public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void Init();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Modifier")
	bool ApplyModifier(UModificationItem* Modifier);
	virtual bool ApplyModifier_Implementation(UModificationItem* Modifier);
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual bool ActivateSkill(AMyMotionController* SkillController);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void ReleaseSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void TickSkill(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void AbortSkill();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void FinishSkill(bool Aborted);
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void StartMontage(AMyMotionController* Controller, UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void StopMontage(AMyMotionController* Controller, UAnimMontage* Montage);

	UFUNCTION(Category = "Default")
	void OnMontageBlendedIn(AMyMotionController* Controller, UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetMontageSection(FName SectionName, AMyMotionController* Controller, UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable, Category = "Gauntlet")
	void ToggleJuwelGlow(bool Glow);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Skill")
	UAnimMontage* SkillMontage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Skill")
	AMyMotionController* SkillController;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Gauntlet")
	UJuwelComponentBase* SkillJuwel;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Gauntlet")
	TArray<UJuwelComponentBase*> ModJuwels;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Modifier")
	USkillAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Modifier")
	TArray<FSkillAttribute> DefaultAttributes;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Skill")
	bool AutoStartMontage = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Skill")
	bool AutoStopMontage = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Skill")
	bool FinishSkillOnRelease = true;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Skill")
	bool TickAfterAbort = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gauntlet")
	bool AutoGlowJuwels = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Skill")
	EActionBlockType BlockType = EActionBlockType::Hand;

protected:
	/*UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void PreActivateSkillExec();
	virtual void PreActivateSkillExec_Implementation() { }*/

	// TODO: Function for Spawning/Destroying additional Actors, ParticleSystems, ...
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void ActivateSkillExec();
	virtual void ActivateSkillExec_Implementation() { }

	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void PostBlendInExec(AMyMotionController* Controller, UAnimMontage* Montage);
	virtual void PostBlendInExec_Implementation(AMyMotionController* Controller, UAnimMontage* Montage) { }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void ReleaseSkillExec();
	virtual void ReleaseSkillExec_Implementation() { }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void AbortSkillExec();
	virtual void AbortSkillExec_Implementation() { }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void TickSkillExec(float DeltaSeconds);
	virtual void TickSkillExec_Implementation(float DeltaSeconds) { }

	// Used to Destroy Actors, that should not persist the skill
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void PostFinishSkillExec(bool Aborted);
	virtual void PostFinishSkillExec_Implementation(bool Aborted) { }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	bool CheckForAbortExec();
	virtual bool CheckForAbortExec_Implementation() { return false; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	bool IsReadyExec();
	virtual bool IsReadyExec_Implementation() { return true; }

protected:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	AActor* SpawnActor(TSubclassOf<AActor> ActorClass, FTransform Transform, AActor* Owner = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Skill|Helper")
	bool LineTraceForObjects(FVector Start, FVector End, FHitResult& OutHit);

	UFUNCTION(BlueprintCallable, Category = "Skill|Helper")
	// bool SphereTraceForObjects(FVector Start, FVector End, float Radius, FHitResult& OutHit);
	bool SphereTraceForObjects(UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);
};
