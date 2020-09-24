// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InteractionSystem : ModuleRules
{
	public InteractionSystem(ReadOnlyTargetRules Target) : base(Target)
	{
        // Override UnrealEngineBuildTool to speedup compilation time
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;

        // Include what you need convention activation
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnforceIWYU = true;

        // Engine dependencies
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore" });

        // The path for the header files
        PublicIncludePaths.AddRange(new string[] { "InteractionSystem/Public" });
        
        // The path for the source files
        PrivateIncludePaths.AddRange(new string[] { "InteractionSystem/Private" });
    }
}
