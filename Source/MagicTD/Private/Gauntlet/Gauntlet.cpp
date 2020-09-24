// Fill out your copyright notice in the Description page of Project Settings.

#include "Gauntlet.h"
#include "JuwelBase.h"
#include "JuwelSocket.h"
#include "Gauntlet/Items/SpellItem.h"
#include "Gauntlet/Items/ModificationItem.h"
#include "MyAssetManager.h"


// Sets default values
AGauntlet::AGauntlet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGauntlet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGauntlet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGauntlet::StartEditing()
{
	TArray<UActorComponent*> JuwelSockets = GetComponentsByClass(UJuwelSocket::StaticClass());
	for (UActorComponent* Socket : JuwelSockets)
	{
		UJuwelSocket* JuwelSocket = Cast<UJuwelSocket>(Socket);
		
		JuwelSocket->OnJuwelAttached.AddDynamic(this, &AGauntlet::OnJuwelAttached);
		JuwelSocket->OnJuwelDettached.AddDynamic(this, &AGauntlet::OnJuwelDettached);
		JuwelSocket->ToggleLocked(false); 
	}
}

void AGauntlet::StopEditing()
{
	TArray<UActorComponent*> JuwelSockets = GetComponentsByClass(UJuwelSocket::StaticClass());
	for (UActorComponent* Socket : JuwelSockets)
	{
		UJuwelSocket* JuwelSocket = Cast<UJuwelSocket>(Socket);

		JuwelSocket->OnJuwelAttached.RemoveDynamic(this, &AGauntlet::OnJuwelAttached);
		JuwelSocket->OnJuwelDettached.RemoveDynamic(this, &AGauntlet::OnJuwelDettached);
		JuwelSocket->ToggleLocked(true);
	}
}

void AGauntlet::OnJuwelAttached(UJuwelSocket* Socket, AJuwelBase* Juwel)
{
	UE_LOG(LogClass, Error, TEXT("Juwel Attached"));

	FSpellConfig& SpellConfig = GauntletConfig.SpellMap.FindOrAdd(Socket->GetSocketType());
	
	if(Socket->GetJuwelType() == UMyAssetManager::SpellItemType)
	{
		SpellConfig.Spell = Cast<USpellItem>(Juwel->GetJuwelItem());
	}
	else 
	{
		SpellConfig.ModificationList.AddUnique(Cast<UModificationItem>(Juwel->GetJuwelItem()));
	}
}

void AGauntlet::OnJuwelDettached(UJuwelSocket* Socket, AJuwelBase* Juwel)
{
	UE_LOG(LogClass, Error, TEXT("Juwel Dettached"));

	FSpellConfig* SpellConfig = GauntletConfig.SpellMap.Find(Socket->GetSocketType());
	if (!SpellConfig)
	{
		UE_LOG(LogClass, Error, TEXT("Can't find SpellConfig for SocketType"));
		return;
	}

	if (Socket->GetJuwelType() == UMyAssetManager::SpellItemType)
	{
		SpellConfig->Spell = nullptr;
	}
	else
	{
		SpellConfig->ModificationList.Remove(Cast<UModificationItem>(Juwel->GetJuwelItem()));
	}
}