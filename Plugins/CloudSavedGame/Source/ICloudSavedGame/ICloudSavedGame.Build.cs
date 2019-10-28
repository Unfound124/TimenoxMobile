// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System.Text;
using Tools.DotNETCommon;

public class ICloudSavedGame : ModuleRules
{
    // Copypaste from UnrealBuildTool/Platform/IOS/UEDeployIOS.cs with modification in iCloud section
    private void WriteEntitlementsFile(ReadOnlyTargetRules Target)
    {
        //get the settings from the ini file
        //plist replacements
        //@todo tvos: Separate TVOS version?
        ConfigHierarchy Ini = ConfigCache.ReadHierarchy(ConfigHierarchyType.Engine, DirectoryReference.FromFile(Target.ProjectFile), Target.Platform);

        bool bCloudKitSupported = false;
        Ini.GetBool("/Script/IOSRuntimeSettings.IOSRuntimeSettings", "bEnableCloudKitSupport", out bCloudKitSupported);

        string BundleID;
        Ini.GetString("/Script/IOSRuntimeSettings.IOSRuntimeSettings", "BundleIdentifier", out BundleID);

        string TeamID;
        Ini.GetString("/Script/CloudSavedGameEditor.CloudSavedGameSettings", "TeamIdentifier", out TeamID);

        bool bForDistribution = Target.Configuration == UnrealTargetConfiguration.Shipping;
        string OutputFilename = Path.Combine(BuildPath(Target), ProjectName(Target) + ".entitlements");

        Directory.CreateDirectory(Path.GetDirectoryName(OutputFilename));
        // we need to have something so Xcode will compile, so we just set the get-task-allow, since we know the value, 
        // which is based on distribution or not (true means debuggable)
        StringBuilder Text = new StringBuilder();
        Text.AppendLine("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        Text.AppendLine("<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">");
        Text.AppendLine("<plist version=\"1.0\">");
        Text.AppendLine("<dict>");
        Text.AppendLine("\t<key>get-task-allow</key>");
        Text.AppendLine(string.Format("\t<{0}/>", bForDistribution ? "false" : "true"));

        if (bCloudKitSupported)
        {
            Text.AppendLine("\t<key>com.apple.developer.icloud-container-identifiers</key>");
            Text.AppendLine("\t<array>");
            Text.AppendLine(string.Format("\t\t<string>iCloud.{0}</string>", BundleID));
            Text.AppendLine("\t</array>");
            Text.AppendLine("\t<key>com.apple.developer.icloud-services</key>");
            Text.AppendLine("\t<array>");
            Text.AppendLine("\t\t<string>CloudKit</string>");
            Text.AppendLine("\t\t<string>CloudDocuments</string>");
            Text.AppendLine("\t</array>");
            Text.AppendLine("\t<key>com.apple.developer.ubiquity-container-identifiers</key>");
            Text.AppendLine("\t<array>");
            Text.AppendLine(string.Format("\t\t<string>iCloud.{0}</string>", BundleID));
            Text.AppendLine("\t</array>");
            Text.AppendLine("\t<key>com.apple.developer.ubiquity-kvstore-identifier</key>");
            Text.AppendLine(string.Format("\t<string>{0}.{1}</string>", TeamID, BundleID));
            Text.AppendLine("\t<key>com.apple.developer.icloud-container-environment</key>");
            Text.AppendLine("\t<array>");
            Text.AppendLine(string.Format("\t\t<string>{0}</string>", bForDistribution ? "Production" : "Development"));
            Text.AppendLine("\t</array>");
        }

        bool bRemoteNotificationsSupported = false;
        Ini.GetBool("/Script/IOSRuntimeSettings.IOSRuntimeSettings", "bEnableRemoteNotificationsSupport", out bRemoteNotificationsSupported);

        if (bRemoteNotificationsSupported)
        {
            Text.AppendLine("\t<key>aps-environment</key>");
            Text.AppendLine(string.Format("\t<string>{0}</string>", bForDistribution ? "production" : "development"));
        }

        Text.AppendLine("</dict>");
        Text.AppendLine("</plist>");

        File.WriteAllText(OutputFilename, Text.ToString());
    }

    private string ProjectPath(ReadOnlyTargetRules Target)
    {
        if (Target.ProjectFile != null)
            return Path.GetDirectoryName(Target.ProjectFile.ToString());

        return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../../"));
    }

    private string BuildPath(ReadOnlyTargetRules Target)
    {
        return Path.Combine(ProjectPath(Target), "Build", Target.Platform.ToString());
    }

    private string ProjectName(ReadOnlyTargetRules Target)
    {
        if (Target.ProjectFile != null)
            return Path.GetFileNameWithoutExtension(Target.ProjectFile.ToString());

        return "";
    }

    public ICloudSavedGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange( new string[] { "Core", "CloudSavedGameCore" } );
        PrivateDependencyModuleNames.AddRange( new string[] { "CoreUObject", "Engine" } );

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicFrameworks.AddRange( new string[] { "SystemConfiguration" } );

            // Generate Entitlements file on-the-fly
            WriteEntitlementsFile(Target);
        }
    }
}
