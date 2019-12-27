// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Timenox : ModuleRules
{
	public Timenox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Playfab", "Json", "JsonUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Playfab", "Json", "JsonUtilities" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		if(Target.Platform == UnrealTargetPlatform.Android){
			PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "OnlineSubsystem" });
			DynamicallyLoadedModuleNames.Add("AndroidAdvertising");
			DynamicallyLoadedModuleNames.Add("OnlineSubsystemGooglePlay");
		}
		if (Target.Platform == UnrealTargetPlatform.IOS){

		}
		// Facebook API
		// PublicDependencyModuleNames.AddRange(new string[] { "Facebook" });
		//Facebook Subsystem
        // PrivateDependencyModuleNames.Add("OnlineSubsystemFacebook");
	}
}
