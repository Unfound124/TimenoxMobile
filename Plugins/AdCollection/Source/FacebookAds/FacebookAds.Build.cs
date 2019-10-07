/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

using System.IO;
using UnrealBuildTool;

public class FacebookAds : ModuleRules
{
    private string EngineMajorVersion;
    private string EngineMinorVersion;
    private string EnginePatchVersion;

    string GetEngineDirectory()
    {
        string magicKey = "UE_ENGINE_DIRECTORY=";
        for(var i = 0; i < PublicDefinitions.Count; i++)
        {
            if(PublicDefinitions[i].IndexOf(magicKey) >= 0)
            {
                return PublicDefinitions[i].Substring(magicKey.Length + 1);
            }
        }

        return "";
    }

    public FacebookAds(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivatePCHHeaderFile = "Public/Facebook.h";
        string strEngineDir = GetEngineDirectory();
        string strEngineVersion = ReadEngineVersion(strEngineDir);

        System.Console.WriteLine("version:" + strEngineVersion);

        /*Definitions.Add( string.Format("ENGINE_VERSION={0}", strEngineVersion) );
        Definitions.Add(string.Format("ENGINE_MAJOR_VERSION={0}", EngineMajorVersion));
        Definitions.Add(string.Format("ENGINE_MINOR_VERSION={0}", EngineMinorVersion));
        Definitions.Add(string.Format("ENGINE_PATCH_VERSION={0}", EnginePatchVersion));*/

        /*PublicIncludePaths.AddRange(
			new string[] {
				"AdMob/Public"
				
				// ... add public include paths required here ...
			}
			);*/

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "AdCollection",

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
        
        

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            //PrivateIncludePaths.Add("Private/Android");
            PrivateDependencyModuleNames.AddRange(
                new string[] {
                "Launch",
                }
                );

            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);

            
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Facebook_UPL.xml"));
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "FBAudienceNetwork",														// Framework name
            "../ThirdParty/ThirdPartyFrameworks/FBAudienceNetwork.embeddedframework.zip")
            );

            PublicAdditionalFrameworks.Add(
            new UEBuildFramework(
            "AdsUtil",														// Framework name
            "../ThirdParty/ThirdPartyFrameworks/AdsUtil.embeddedframework.zip")
            );


            PublicFrameworks.AddRange(
            new string[]
            {
            "EventKit",
            "MediaPlayer",
            "AdSupport",
            "CoreLocation",
            "SystemConfiguration",
            "MessageUI",
            "Security",
            "CoreTelephony"
            }
            );
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Win64)
        {
            //PrivateIncludePaths.Add("Private/Windows");
        }
        else if(Target.Platform == UnrealTargetPlatform.Mac)
        {
            //PrivateIncludePaths.Add("Private/Mac");
        }
        else
        {
            PrecompileForTargets = PrecompileTargetsType.None;
        }
    }

    private string ReadEngineVersion(string EngineDirectory)
    {
        string EngineVersionFile = Path.Combine(EngineDirectory, "Runtime", "Launch", "Resources", "Version.h");
        string[] EngineVersionLines = File.ReadAllLines(EngineVersionFile);
        for (int i = 0; i < EngineVersionLines.Length; ++i)
        {
            if (EngineVersionLines[i].StartsWith("#define ENGINE_MAJOR_VERSION"))
            {
                EngineMajorVersion = EngineVersionLines[i].Split('\t')[1].Trim(' ');
            }
            else if (EngineVersionLines[i].StartsWith("#define ENGINE_MINOR_VERSION"))
            {
                EngineMinorVersion = EngineVersionLines[i].Split('\t')[1].Trim(' ');
            }
            else if (EngineVersionLines[i].StartsWith("#define ENGINE_PATCH_VERSION"))
            {
                EnginePatchVersion = EngineVersionLines[i].Split('\t')[1].Trim(' ');
            }
            
        }

        return EngineMajorVersion + "." + EngineMinorVersion + "." + EnginePatchVersion;
    }
    
}
