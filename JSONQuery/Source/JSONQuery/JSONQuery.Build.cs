// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;

public class JSONQuery : ModuleRules
{
	public JSONQuery(ReadOnlyTargetRules Target):base(Target)
	{
        PublicDependencyModuleNames.AddRange(
			new string[] { 
				"Core", 
                "HTTP",
				"CoreUObject",
				"Engine",
                "Json"
			}
		);
	}
}
