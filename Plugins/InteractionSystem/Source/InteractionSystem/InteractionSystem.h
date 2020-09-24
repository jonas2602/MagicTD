// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

/* Use this if developing a plugin */
//#pragma once
//
//#include "CoreMinimal.h"
//#include "ModuleManager.h"
//
//class FInteractionSystemModule : public IModuleInterface
//{
//public:
//
//	/** IModuleInterface implementation */
//	virtual void StartupModule() override;
//	virtual void ShutdownModule() override;
//};


/* use this if developing a game module */
#pragma once

#include "ModuleManager.h"

/* use in both */
INTERACTIONSYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogInteraction, Log, All);