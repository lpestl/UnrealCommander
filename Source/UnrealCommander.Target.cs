// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Desktop)]
[SupportedPlatforms("IOS")]
public class UnrealCommanderTarget : TargetRules
{
	public UnrealCommanderTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Program;
		LinkType = TargetLinkType.Monolithic;

		// TODO: What kind of animal is it and with what is it eaten?
		DefaultBuildSettings = BuildSettingsVersion.V2;

		LaunchModuleName = "UnrealCommander";
		ExtraModuleNames.AddRange( 
			new string[]
			{
				"UnrealCommander", 
				"EditorStyle"
			} );

		bBuildDeveloperTools = false;

		// UnrealCommander doesn't ever compile with the engine linked in
		bCompileAgainstEngine = false;

		// We need CoreUObject compiled in as the source code access module requires it
		bCompileAgainstCoreUObject = true;

		// UnrealCommander.exe has no exports, so no need to verify that a .lib and .exp file was emitted by
		// the linker.
		bHasExports = false;

		// Make sure to get all code in SlateEditorStyle compiled in
        bBuildDeveloperTools = true;
	}
}
