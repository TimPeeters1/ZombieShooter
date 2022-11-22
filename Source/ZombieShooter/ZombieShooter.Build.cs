

using UnrealBuildTool;

public class ZombieShooter : ModuleRules
{
    public ZombieShooter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", "CoreUObject", 
            "Engine",
            "InputCore",
            "AIModule",
            "GameplayTasks",    
            "MetasoundEngine",
            "Niagara",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",});


        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
