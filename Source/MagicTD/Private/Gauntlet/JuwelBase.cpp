// Fill out your copyright notice in the Description page of Project Settings.

#include "JuwelBase.h"
#include "JuwelSocket.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"
#include "Engine/World.h"
#include "Gauntlet/Items/JuwelItem.h"

// Sets default values
AJuwelBase::AJuwelBase()
	: SearchSocket(false)
	, JuwelState(EJuwelState::Free)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	JuwelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JuwelMesh"));
	JuwelMesh->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	SetRootComponent(JuwelMesh);
}

AJuwelBase* AJuwelBase::CreateJuwel(UObject* WorldContextObject, UJuwelItem* Template)
{
	if (!Template)
	{
		UE_LOG(LogClass, Error, TEXT("Template not Valid"));
		return nullptr;
	}

	// Get World
	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		UE_LOG(LogClass, Error, TEXT("World not Valid"));
		return nullptr;
	}

	// Init Juwel
	FActorSpawnParameters SpawnParams;
	AJuwelBase* Juwel = World->SpawnActor<AJuwelBase>(Template->JuwelClass, FTransform::Identity, SpawnParams);
	Juwel->Init(Template);

	return Juwel;
}

AJuwelBase* AJuwelBase::CreateJuwelAttached(UObject* WorldContextObject, UJuwelItem* Template, UJuwelSocket* Socket)
{
	AJuwelBase* Juwel = CreateJuwel(WorldContextObject, Template);
	if (Juwel && Socket)
	{
		Juwel->JuwelMesh->SetSimulatePhysics(false);
		Socket->TryAttachJuwel(Juwel);
	}

	return Juwel;
}

FPrimaryAssetType AJuwelBase::GetItemType() 
{
	return JuwelItem->ItemType; 
}

// Called when the game starts or when spawned
void AJuwelBase::BeginPlay()
{
	Super::BeginPlay();

	if (!PullEmitterTemplate) { UE_LOG(LogClass, Error, TEXT("PullEmitterTemplate not Setup!")); }
	if (!LerpCurve) { UE_LOG(LogClass, Error, TEXT("LerpCurve not Setup!")); }
}

// Called every frame
void AJuwelBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SearchSocket)
	{
		if (InUse())
		{
			UpdateClosestSocket();
		}

		if (ClosestSocket)
		{
			UpdateParticles();
		}
	}

	if (!InUse() && ClosestSocket)
	{
		LerpToTarget(DeltaTime);
	}
}

void AJuwelBase::ActivatePull()
{
	PullEmitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PullEmitterTemplate, FTransform::Identity, false);

	JuwelMesh->OnComponentBeginOverlap.AddDynamic(this, &AJuwelBase::OnJuwelMeshBeginOverlapHandle);
	JuwelMesh->OnComponentEndOverlap.AddDynamic(this, &AJuwelBase::OnJuwelMeshEndOverlapHandle);

	SearchSocket = true;
}

void AJuwelBase::DeactivatePull()
{
	SearchSocket = false;

	JuwelMesh->OnComponentBeginOverlap.RemoveDynamic(this, &AJuwelBase::OnJuwelMeshBeginOverlapHandle);
	JuwelMesh->OnComponentEndOverlap.RemoveDynamic(this, &AJuwelBase::OnJuwelMeshEndOverlapHandle);

	PullEmitter->DestroyComponent();
	ClosestSocket = nullptr;
}

void AJuwelBase::ToggleLocked(bool Locked)
{
	// Can't lock free Juwel
	if (JuwelState == EJuwelState::Free) return;

	if (Locked)
	{
		JuwelMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		JuwelState = EJuwelState::Locked;
	}
	else
	{
		JuwelMesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
		JuwelState = EJuwelState::Attached;
	}
}

bool AJuwelBase::OnPickup_Implementation(AActor* Interactor)
{
	ActivatePull();
	JuwelState = EJuwelState::Free;

	return true;
}

bool AJuwelBase::OnDrop_Implementation()
{
	if (ClosestSocket)
	{
		JuwelMesh->SetSimulatePhysics(false);
	}
	else
	{
		DeactivatePull();
	}

	return true;
}

bool AJuwelBase::CanInteractExec_Implementation(AActor* Interactor)
{
	return JuwelState != EJuwelState::Locked;
}

void AJuwelBase::OnJuwelMeshBeginOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Is JuwelSocket?
	UJuwelSocket* Socket = Cast<UJuwelSocket>(OtherComp);
	if (Socket && Socket->IsJuwelAttachable(this))
	{
		OverlappingSockets.AddUnique(Socket);
	}
}

void AJuwelBase::OnJuwelMeshEndOverlapHandle(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Is JuwelSocket?
	UJuwelSocket* Socket = Cast<UJuwelSocket>(OtherComp);
	if (Socket)
	{
		OverlappingSockets.Remove(Socket);
	}
}

void AJuwelBase::LerpToTarget(float DeltaTime)
{
	FVector JuwelToSocket = ClosestSocket->GetComponentLocation() - GetActorLocation();
	float Distance = JuwelToSocket.Size();

	if (Distance > 0)
	{
		float LerpSpeed = LerpCurve->GetFloatValue(Distance) * DeltaTime;

		FVector NewLocation = GetActorLocation() + JuwelToSocket.GetSafeNormal() * FMath::Min(LerpSpeed, Distance);

		SetActorLocation(NewLocation);
	}
	else
	{
		// Reached Socket
		if (!ClosestSocket->TryAttachJuwel(this))
		{
			UE_LOG(LogClass, Error, TEXT("Attach failed"));
			return;
		}

		DeactivatePull();
		JuwelState = EJuwelState::Attached;
	}
}

void AJuwelBase::UpdateParticles()
{
	FTransform EmitterTransform = PullEmitter->GetRelativeTransform();
	FVector SourceRelativeLocation = EmitterTransform.InverseTransformPosition(GetActorLocation());

	PullEmitter->SetVectorParameter("SourceLocation", SourceRelativeLocation);
}

void AJuwelBase::UpdateClosestSocket()
{
	UJuwelSocket* NewClosestSocket = FindClosestSocket();
	if (NewClosestSocket != ClosestSocket)
	{

		if (NewClosestSocket && !ClosestSocket)
		{
			PullEmitter->Activate();
		}

		if (!NewClosestSocket && ClosestSocket)
		{
			PullEmitter->Deactivate();
		}

		ClosestSocket = NewClosestSocket;
		if (ClosestSocket)
		{
			PullEmitter->SetWorldLocation(NewClosestSocket->GetComponentLocation());
		}
	}
}

UJuwelSocket* AJuwelBase::FindClosestSocket()
{
	UJuwelSocket* ClosestSocket = nullptr;
	float BestDistance = -1;

	for (UJuwelSocket* Socket : OverlappingSockets)
	{
		FVector JuwelToSocket = GetActorLocation() - Socket->GetComponentLocation();
		float Distance = JuwelToSocket.Size();

		if (Distance < BestDistance || BestDistance == -1)
		{
			ClosestSocket = Socket;
			BestDistance = Distance;
		}
	}

	return ClosestSocket;
}
