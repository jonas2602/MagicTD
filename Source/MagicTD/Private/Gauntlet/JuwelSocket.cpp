// Fill out your copyright notice in the Description page of Project Settings.

#include "JuwelSocket.h"
#include "Engine/CollisionProfile.h"
#include "JuwelItem.h"
#include "JuwelBase.h"

// Called when the game starts
void UJuwelSocket::BeginPlay()
{
	Super::BeginPlay();

}

bool UJuwelSocket::TryAttachJuwel(AJuwelBase* Juwel)
{
	if (!IsJuwelAttachable(Juwel)) { return false; }

	Juwel->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OnJuwelAttached.Broadcast(this, Juwel);

	AttachedJuwel = Juwel;
	AttachedJuwel->ToggleLocked(Locked);
	AttachedJuwel->OnInteractionStarted.AddDynamic(this, &UJuwelSocket::OnJuwelGrabbed);

	return true;
}

void UJuwelSocket::ToggleLocked(bool Locked)
{
	if (Locked)
	{
		SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}
	else
	{
		SetCollisionProfileName("OverlapAll");
	}
	this->Locked = Locked;

	if (AttachedJuwel)
	{
		AttachedJuwel->ToggleLocked(Locked);
	}
}

void UJuwelSocket::OnJuwelGrabbed(AInteractableBase* Interactable)
{
	AttachedJuwel->OnInteractionStarted.RemoveDynamic(this, &UJuwelSocket::OnJuwelGrabbed);
	OnJuwelDettached.Broadcast(this, AttachedJuwel);
	AttachedJuwel = nullptr;
}

bool UJuwelSocket::IsJuwelAttachable_Implementation(AJuwelBase* Juwel)
{
	if (AttachedJuwel)
	{
		//UE_LOG(LogClass, Warning, TEXT("Can't attach Juwel because slot is already used"));
		return false;
	}

	if (JuwelType != Juwel->GetJuwelItem()->ItemType)
	{
		//UE_LOG(LogClass, Warning, TEXT("Can't attach Juwel because ItemType is not allowed (%s != %s)"), *JuwelType.ToString(), *Juwel->GetJuwelItem()->ItemType.GetName().ToString());
		return false;
	}

	return true;
}