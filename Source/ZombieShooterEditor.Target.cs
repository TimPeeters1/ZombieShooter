

using UnrealBuildTool;
using System.Collections.Generic;

public class ZombieShooterEditorTarget : TargetRules
{
	public ZombieShooterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ZombieShooter" } );
    }
}
