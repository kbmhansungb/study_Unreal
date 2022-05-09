// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CharacterAnimationEditorTarget : TargetRules
{
	public CharacterAnimationEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("CharacterAnimation");

		ExtraModuleNames.Add("BuildingConstruction");
		ExtraModuleNames.Add("InteractiveObject");
		ExtraModuleNames.Add("ProceduralMeshLibrary");
		ExtraModuleNames.Add("UITool");
        ExtraModuleNames.Add("Rendering");
    }
}
