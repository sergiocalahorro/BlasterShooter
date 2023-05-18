// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BlasterShooter : ModuleRules
{
	public BlasterShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"EnhancedInput",
			"OnlineSubsystem",
			"OnlineSubsystemSteam"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{ 
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});
	}
}
