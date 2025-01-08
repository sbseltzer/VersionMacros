// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VersionMacros : ModuleRules
{
#if UE_4_17_OR_LATER
	public VersionMacros(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.Add("Core");
		PrivateDependencyModuleNames.Add("CoreUObject");
	}
#else
	// NOTE: This is flagged as obsolete in 4.16 but the UE_*_OR_LATER C# macros weren't introduced until 4.17
	public VersionMacros(TargetInfo Target)
	{
		PublicDependencyModuleNames.Add("Core");
		PrivateDependencyModuleNames.Add("CoreUObject");
	}
#endif
}

