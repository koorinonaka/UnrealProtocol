// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class UnrealProtocolEd : ModuleRules
{
	public UnrealProtocolEd(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",

			"Slate",
			"SlateCore",

			"UnrealEd",

			"UnrealProtocol"
		});
	}
}