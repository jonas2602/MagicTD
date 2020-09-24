// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

/* Use this if developing a plugin */
//#include "InteractionSystem.h"
//
//#define LOCTEXT_NAMESPACE "FInteractionSystemModule"
//
//void FInteractionSystemModule::StartupModule()
//{
//	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
//}
//
//void FInteractionSystemModule::ShutdownModule()
//{
//	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
//	// we call this function before unloading the module.
//}
//
//#undef LOCTEXT_NAMESPACE
//	
//IMPLEMENT_MODULE(FInteractionSystemModule, InteractionSystem)


/* use this if developing a game module */
#include "InteractionSystem.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, InteractionSystem, "InteractionSystem");

/* use in both */

/** Logging definitions */
DEFINE_LOG_CATEGORY(LogInteraction);