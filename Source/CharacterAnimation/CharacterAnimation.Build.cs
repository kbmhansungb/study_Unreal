// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CharacterAnimation : ModuleRules
{
	public CharacterAnimation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",

			"RHI",
			"RenderCore",
			"Projects"
		});
	}
}
