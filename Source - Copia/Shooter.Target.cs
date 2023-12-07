// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShooterTarget : TargetRules
{
	public ShooterTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		ExtraModuleNames.AddRange( new string[] { "Shooter" } );

		bUseUnityBuild = true;
	}
}
