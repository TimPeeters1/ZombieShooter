

using UnrealBuildTool;
using System.Collections.Generic;

public class ZombieShooterTarget : TargetRules
{
	public ZombieShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ZombieShooter" } );
	}
}
