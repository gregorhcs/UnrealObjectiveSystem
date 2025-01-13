// Copyright 2024 Thieves of the Night. All Rights Reserved.

using UnrealBuildTool;

public class ObjectiveSystem : ModuleRules
{
	public ObjectiveSystem(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core", 
				"GameplayTags", 
				"WeekendUtils",
				"DeveloperSettings",
				"ModelViewViewModel",
				"UMG"
			}
		);
			
		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"GameplayTags",
				"UIRoot"
			}
		);
	}
}
