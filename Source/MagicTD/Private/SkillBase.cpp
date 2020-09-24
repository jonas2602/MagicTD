// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillBase.h"
#include "MotionController.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Gauntlet/JuwelComponentBase.h"
#include "SpellSystemInterface.h"
#include "ModificationItem.h"
#include "SkillTypes.h"
#include "Engine/World.h"
#include "TimerManager.h"
//#include "Runtime/Engine/Private/KismetTraceUtils.h"

// Sets default values for this component's properties
USkillBase::USkillBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...

	// Create the attribute set, this replicates by default
	// AttributeSet = NewObject<USkillAttributeSet>(this, TEXT("AttributeSet"));
	// UE_LOG(LogClass, Error, TEXT("Constructor, %s"), AttributeSet != nullptr ? *FName(TEXT("TRUE")).ToString() : *FName(TEXT("FALSE")).ToString());
}

//UWorld* USkillBase::GetWorld() const
//{
//	UWorld* ComponentWorld = nullptr;
//
//	// Default: SkillController is already valid
//	if (SkillController)
//	{
//		ComponentWorld = SkillController->GetWorld();
//	}
//
//	// try to get it from outer
//	if (!ComponentWorld)
//	{
//		ComponentWorld = GetOuter()->GetWorld();
//	}
//
//	// Fallback: try to cast
//	if (!ComponentWorld)
//	{
//		ComponentWorld = Cast<UWorld>(GetOuter());
//	}
//
//	return ComponentWorld;
//}

UAnimInstance* USkillBase::GetAnimBP(AMyMotionController* Controller)
{
	if (!Controller) return nullptr;

	return Controller->HandMesh->GetAnimInstance();
}

USceneComponent* USkillBase::GetProjectileSocket(AMyMotionController* Controller)
{
	if (!Controller) return nullptr;

	return Controller->ProjectileSocket;
}

AGauntlet* USkillBase::GetGauntlet(AMyMotionController* Controller)
{
	if (!Controller) return nullptr;
	if (!Controller->Implements<USpellSystemInterface>())
	{
		UE_LOG(LogClass, Error, TEXT("Controller doesn't implement SpellSystemInterface"));
		return nullptr;
	}

	return ISpellSystemInterface::Execute_GetGauntlet(Controller);
}

bool USkillBase::GetSkillJuwels(UJuwelComponentBase*& OutSpellJuwel, TArray<UJuwelComponentBase*>& OutModJuwels)
{
	if (!SkillController) return false;
	if (!SkillController->Implements<USpellSystemInterface>())
	{
		UE_LOG(LogClass, Error, TEXT("Controller doesn't implement SpellSystemInterface"));
		return false;
	}

	return ISpellSystemInterface::Execute_GetJuwelsToSpell(SkillController, GetClass(), OutSpellJuwel, OutModJuwels);
}

void USkillBase::StartMontage(AMyMotionController* Controller, UAnimMontage* Montage)
{
	if (!Montage) return;
	if (!Controller) return;

	UAnimInstance* AnimInstance = GetAnimBP(Controller);

	// BlendTime will only be correct, when checked after start of the montage
	AnimInstance->Montage_Play(Montage);
	float BlendTime = AnimInstance->Montage_GetBlendTime(Montage);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("OnMontageBlendedIn"), Controller, Montage);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, BlendTime, false);
}

void USkillBase::StopMontage(AMyMotionController* Controller, UAnimMontage* Montage)
{
	if (!Montage) return;
	if (!Controller) return;

	GetAnimBP(Controller)->Montage_Stop(0.2f, Montage);
}

void USkillBase::OnMontageBlendedIn(AMyMotionController* Controller, UAnimMontage* Montage)
{
	if (!Controller) return;

	PostBlendInExec(Controller, Montage);
}

void USkillBase::SetMontageSection(FName SectionName, AMyMotionController* Controller, UAnimMontage* Montage)
{
	if (!Montage) return;
	if (!Controller) return;

	GetAnimBP(Controller)->Montage_SetNextSection(GetAnimBP(Controller)->Montage_GetCurrentSection(), SectionName, Montage);
}

void USkillBase::ToggleJuwelGlow(bool Glow)
{
	if (SkillJuwel)
	{
		SkillJuwel->ToggleGlow(Glow);
	}

	for (UJuwelComponentBase* Juwel : ModJuwels)
	{
		if (Juwel)
		{
			Juwel->ToggleGlow(Glow);
		}
	}
}

/***************************** End Helper **********************************/

void USkillBase::Init()
{
	AttributeSet = NewObject<USkillAttributeSet>(this, TEXT("AttributeSet"));

	for (FSkillAttribute Attribute : DefaultAttributes)
	{
		AttributeSet->Insert(Attribute);
	}
}

bool USkillBase::ApplyModifier_Implementation(UModificationItem* Modifier)
{
	bool AllSucceeded = true;
	for (FSkillAttributeModifier AttributeModifier : Modifier->AttributeModifierList)
	{
		if (!AttributeSet->Adjust(AttributeModifier))
		{
			UE_LOG(LogClass, Error, TEXT("Unable to Adjust %s"), *AttributeModifier.AttributeName.ToString());
			AllSucceeded = false;
		}
	}

	return AllSucceeded;
}

bool USkillBase::ActivateSkill(AMyMotionController* SkillController)
{
	// Required before check, because isready may use it
	this->SkillController = SkillController;

	// Break when not ready
	if (!IsReadyExec()) return false;

	if (AutoStartMontage)
	{
		StartMontage(SkillController, SkillMontage);
	}

	// Load Juwel Refs
	if (!GetSkillJuwels(SkillJuwel, ModJuwels))
	{
		UE_LOG(LogClass, Error, TEXT("Failed to Load Juwels"));
	}
	else if (AutoGlowJuwels)
	{
		// Enable Glow if requested
		ToggleJuwelGlow(true);
	}

	ActivateSkillExec();
	return true;
}

void USkillBase::ReleaseSkill()
{
	ReleaseSkillExec();

	if (FinishSkillOnRelease)
	{
		FinishSkill(false);
	}
}

void USkillBase::TickSkill(float DeltaTime)
{
	if (CheckForAbortExec())
	{
		AbortSkill();
		if (!TickAfterAbort) return;
	}

	TickSkillExec(DeltaTime);
}


void USkillBase::AbortSkill()
{
	AbortSkillExec();

	FinishSkill(true);
}

void USkillBase::FinishSkill(bool Aborted)
{
	if (AutoStopMontage)
	{
		StopMontage(SkillController, SkillMontage);
	}

	if (AutoGlowJuwels)
	{
		// Disable Glow if requested
		ToggleJuwelGlow(false);
	}

	OnFinished.Broadcast(this);
	PostFinishSkillExec(Aborted);

	this->SkillController = nullptr;
}


/**************************** Helper for Blueprint *********************************/
AActor* USkillBase::SpawnActor(TSubclassOf<AActor> ActorClass, FTransform Transform, AActor* Owner)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = Owner ? Owner : SkillController;
	AActor* NewActor = Owner->GetWorld()->SpawnActor<AActor>(ActorClass, Transform, SpawnInfo);

	return NewActor;
}

bool USkillBase::LineTraceForObjects(FVector Start, FVector End, FHitResult& OutHit)
{
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")));
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);
	GetWorld()->LineTraceSingleByObjectType(
		RV_Hit,
		Start,
		End,
		FCollisionObjectQueryParams::DefaultObjectQueryParam,
		RV_TraceParams
	);

	if (RV_Hit.GetActor())
	{
		OutHit = RV_Hit;
		return true;
	}

	return false;
}

bool USkillBase::SphereTraceForObjects(UObject* WorldContextObject, const FVector Start, const FVector End, float Radius, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
// bool USkillBase::SphereTraceForObjects(FVector Start, FVector End, float Radius, FHitResult& OutHit)
{
	return UKismetSystemLibrary::SphereTraceSingleForObjects(WorldContextObject, Start, End, Radius, ObjectTypes, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);
	/*static const FName SphereTraceSingleName(TEXT("SphereTraceSingleForObjects"));
	FCollisionQueryParams Params = ConfigureCollisionParams(SphereTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);

	FCollisionObjectQueryParams ObjectParams = ConfigureCollisionObjectParams(ObjectTypes);
	if (ObjectParams.IsValid() == false)
	{
		UE_LOG(LogBlueprintUserMessages, Warning, TEXT("Invalid object types"));
		return false;
	}

	UWorld* World = WorldContextObject->GetWorld();
	bool const bHit = World ? World->SweepSingleByObjectType(OutHit, Start, End, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(Radius), Params) : false;

#if ENABLE_DRAW_DEBUG
	DrawDebugSphereTraceSingle(World, Start, End, Radius, DrawDebugType, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
#endif

	return bHit;

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")));
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	FHitResult RV_Hit(ForceInit);
	GetWorld()->SweepSingleByObjectType(
		RV_Hit,
		Start,
		End,
		FQuat(),
		FCollisionObjectQueryParams::DefaultObjectQueryParam,
		FCollisionShape::MakeSphere(Radius),
		RV_TraceParams
	);

	if (RV_Hit.GetActor())
	{
		OutHit = RV_Hit;
		return true;
	}

	return false;*/
}