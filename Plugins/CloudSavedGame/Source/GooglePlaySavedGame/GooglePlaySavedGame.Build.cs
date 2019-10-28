// Google Play Saved Games
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class GooglePlaySavedGame : ModuleRules
{
    public GooglePlaySavedGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange( new string[] { "Core", "CloudSavedGameCore" } );
		PrivateDependencyModuleNames.AddRange( new string[] { "CoreUObject", "Engine" } );
		
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            /** Add Definition to use protected members of OnlineSubsystemGooglePlay as public */
            PublicDefinitions.Add("ONLINESUBSYSTEMGOOGLEPLAY_PACKAGE=1");

            string EnginePath = Path.GetFullPath(Target.RelativeEnginePath);
            string GooglePlayPath = EnginePath + "Plugins/Online/Android/OnlineSubsystemGooglePlay/Source/";

            /** Need Private Path of OnlineSubsystemGooglePlay cause have dependencies in head file */
            PublicIncludePaths.Add(GooglePlayPath + "Private");
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "OnlineSubsystem",
                    "OnlineSubsystemGooglePlay",
                    "GpgCppSDK",
                    "Launch"
                }
            );

            /* Add Android UPL Receipt */
            string ModulePath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModulePath, "GooglePlaySavedGame_UPL_Android.xml"));
            /* ~Add Android UPL Receipt */
        }
    }
}
