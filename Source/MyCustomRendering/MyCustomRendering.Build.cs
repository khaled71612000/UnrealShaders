// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class MyCustomRendering : ModuleRules
{
	public MyCustomRendering(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "Renderer",
            "RenderCore",
            "RHI"
        });

        //so we can reference some private engine classes in our implementation later.
        string EnginePath = Path.GetFullPath(Target.RelativeEnginePath);
        PublicIncludePaths.Add(EnginePath + "Source/Runtime/Renderer/Private");

        PrivateDefinitions.Add("MYCUSTOMRENDERING_EXPORTS=1");

    }
}