// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "UsableBase.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API AUsableBase : public AInteractableBase
{
	GENERATED_BODY()

public:
	// Not necessary for VR?
	// TODO: Pipe input to Interactor, Control over player returns, when release called
	
};
