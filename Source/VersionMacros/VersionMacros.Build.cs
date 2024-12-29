// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VersionMacros : ModuleRules
{
	public VersionMacros(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
	}
}
