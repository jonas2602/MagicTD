// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupBase.h"
#include "Components/MeshComponent.h"
#include "InteractorInterface.h"


// Sets default values
APickupBase::APickupBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	InteractorSwitchable = true;

	//// Create Root Component
	//RootBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root"));
	//SetRootComponent(RootBillboard);

	// Create the static mesh component
	/*PickupMesh = CreateDefaultSubobject<UMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetEnableGravity(true);
	SetRootComponent(PickupMesh);*/
}

// TODO: implement Pickup can be switched between interactors
bool APickupBase::InteractExec_Implementation(AActor* Interactor)
{
	if (!Interactor->Implements<UInteractorInterface>())
	{
		UE_LOG(LogInteraction, Error, TEXT("Interacting Actor does not implement InteractorInterface, which is required for Pickups"));
		return false;
	}

	UMeshComponent* Mesh = GetMeshComponent();
	if (!Mesh)
	{
		UE_LOG(LogInteraction, Error, TEXT("GetMeshComponent() not Setup on %s"), *GetName());
		return false;
	}
	Mesh->SetSimulatePhysics(false);

	// Get Socket of Interactor to Parent to
	FName SocketName = NAME_None;
	USceneComponent* PickupSocket = IInteractorInterface::Execute_GetPickupParent(Interactor, SocketName);
	if (PickupSocket == nullptr)
	{
		UE_LOG(LogInteraction, Error, TEXT("PickupSocket invalid"));
		return false;
	}

	// Attach self to interactor
	AttachToComponent(PickupSocket, FAttachmentTransformRules(AttachmentRule, true), SocketName);
	if (AttachmentRule == EAttachmentRule::SnapToTarget)
	{
		SetActorRelativeTransform(RelativeTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}

	return OnPickup(Interactor);
}

bool APickupBase::ReleaseExec_Implementation()
{
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));

	UMeshComponent* Mesh = GetMeshComponent();
	if (!Mesh)
	{
		UE_LOG(LogInteraction, Error, TEXT("GetMeshComponent() not Setup on %s"), *GetName());
		return false;
	}
	Mesh->SetSimulatePhysics(true);

	return OnDrop();
}
