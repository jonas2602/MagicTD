// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableBase.h"
#include "InteractionWidgetComponent.h"
#include "InteractionSocketComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/MeshComponent.h"
#include "InteractorInterface.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Engine/World.h"
#include "TimerManager.h"


// Sets default values
AInteractableBase::AInteractableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//// Create Root Component
	//RootBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root"));
	//SetRootComponent(RootBillboard);

	//// Create the static mesh component
	//InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	//InteractableMesh->SetupAttachment(RootComponent);

	//if (HighlightType == EHighlightType::Widget)
	//{
	//	// Create the Interaction Widget components
	//	InteractableWidget = CreateDefaultSubobject<UInteractionWidgetComponent>(TEXT("InteractableWidget"));
	//	InteractableWidget->SetupAttachment(RootComponent);
	//}
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	// Try to fill InteractionWidget and InteractionSocket
	InteractableWidget = FindComponentByClass<UInteractionWidgetComponent>();
	if (!InteractableWidget)
	{
		UE_LOG(LogInteraction, Warning, TEXT("Couldn't find InteractionWidget on %s. Meshes will get outline when interactable"), *GetName());
	}

	InteractableSocket = FindComponentByClass<UInteractionSocketComponent>();
	if (!InteractableSocket)
	{
		UE_LOG(LogInteraction, Warning, TEXT("Couldn't find InteractionSocket on %s. InteractionLocation will be Actororigin"), *GetName());
	}

	// Set Outline Color
	TArray<UMeshComponent*> MeshComponents;
	GetOutlineMeshes(MeshComponents);
	for (UMeshComponent* Mesh : MeshComponents)
	{
		Mesh->SetCustomDepthStencilValue((int)OutlineColor);
	}

	ToggleHighlight(false);
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InUse() && InteractionTickEnabled)
	{
		InteractionTick(DeltaTime);
	}
}

/************ Interaction Interface Start *************/
bool AInteractableBase::Interact(AActor* Interactor)
{
	if (Interactor == nullptr)
	{
		UE_LOG(LogInteraction, Error, TEXT("Interactor not valid"));
		return false;
	}

	UE_LOG(LogInteraction, Warning, TEXT("%s triggered Interaction with %s"), *Interactor->GetName(), *GetName());

	if (ActiveInteractor)
	{
		if (InteractorSwitchable)
		{
			AActor* InteractorCache = ActiveInteractor;
			if (!Release(ActiveInteractor))
			{
				return false;
			}
			IInteractorInterface::Execute_ForceRelease(InteractorCache, this);
			UE_LOG(LogInteraction, Warning, TEXT("Forced %s to release %s"), *InteractorCache->GetName(), *GetName());
		}
		else
		{
			UE_LOG(LogInteraction, Warning, TEXT("%s is already in use"), *GetName());
			return false;
		}
	}

	if (InteractExec(Interactor))
	{
		this->ActiveInteractor = Interactor;

		if (AutoHandleMontage && InteractorMontage)
		{
			PlayMontageOnInteractor(InteractorMontage);
		}

		OnInteractionStarted.Broadcast(this);
		return true;
	}

	UE_LOG(LogInteraction, Warning, TEXT("Interaction with %s failed"), *GetName());
	return false;
}

bool AInteractableBase::Release(AActor* Interactor)
{
	if (!Interactor || this->ActiveInteractor != Interactor)
	{
		UE_LOG(LogInteraction, Error, TEXT("Actor can't release when he is not interacting"));
		return false;
	}

	UE_LOG(LogInteraction, Warning, TEXT("%s triggered Release with %s"), *Interactor->GetName(), *GetName());
	if (ReleaseExec())
	{
		if (AutoHandleMontage && InteractorMontage)
		{
			StopMontageOnInteractor(InteractorMontage);
		}

		this->ActiveInteractor = nullptr;
		OnInteractionFinished.Broadcast(this);
		return true;
	}

	return false;
}

bool AInteractableBase::CanInteract(AActor* Interactor)
{
	// Interactor Valid?
	if (Interactor == nullptr)
	{
		return false;
	}

	// Allowed to Switch when already in use?
	if (InUse() && !InteractorSwitchable)
	{
		return false;
	}

	return CanInteractExec(Interactor);
}

void AInteractableBase::ToggleHighlight(bool Highlight)
{
	if (InteractableWidget)
	{
		// If widget exists it must be shown/hidden ...
		InteractableWidget->SetVisibility(Highlight);
	}
	else
	{
		// ... else meshes will be outlined
		TArray<UMeshComponent*> MeshComponents;
		GetOutlineMeshes(MeshComponents); // TODO: Use Tags to determine which components to highlight?
		for (UMeshComponent* Mesh : MeshComponents)
		{
			Mesh->SetRenderCustomDepth(Highlight);
		}
	}
}

void AInteractableBase::GetOutlineMeshes_Implementation(TArray<UMeshComponent*>& OutComponents)
{
	GetComponents<UMeshComponent>(OutComponents);
}

FVector AInteractableBase::GetInteractionLocation()
{
	if (InteractableSocket)
	{
		return InteractableSocket->GetComponentLocation();
	}
	else
	{
		// InteractionLocation will be Actororigin
		return GetActorLocation();
	}
}

float AInteractableBase::GetInteractionMaxDistance()
{
	return InteractionMaxDistance;
}
/************* Interaction Interface End **************/

void AInteractableBase::PlayMontageOnInteractor(UAnimMontage* Montage)
{
	if (!Montage) return;
	if (!ActiveInteractor) return;

	UAnimInstance* AnimInstance = IInteractorInterface::Execute_GetAnimBP(ActiveInteractor);
	if (AnimInstance)
	{
		// BlendTime will only be correct, when checked after start of the montage
		AnimInstance->Montage_Play(Montage);
		float BlendTime = AnimInstance->Montage_GetBlendTime(Montage);

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("OnMontageBlendedIn"), Montage);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, BlendTime, false);
	}
}

void AInteractableBase::StopMontageOnInteractor(UAnimMontage* Montage)
{
	if (!Montage) return;
	if (!ActiveInteractor) return;

	UAnimInstance* AnimInstance = IInteractorInterface::Execute_GetAnimBP(ActiveInteractor);
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0, Montage);
	}
}