// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Rendering : ModuleRules
{
    public Rendering(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",

            "RHI",
            "RenderCore"
        });
    }
}
