// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#include "CloudSavedGameEditorModule.h"
#include "CloudSavedGameSettings.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif

#define LOCTEXT_NAMESPACE "FCloudSavedGameEditorModule"

void FCloudSavedGameEditorModule::StartupModule()
{
	RegisterSettings();
}

void FCloudSavedGameEditorModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		UnregisterSettings();
	}
}

void FCloudSavedGameEditorModule::RegisterSettings()
{
#if WITH_EDITOR
	// Register settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "CloudSavedGame",
			LOCTEXT("RuntimeSettingsName", "Cloud Saved Game"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Cloud Saved Game plugin"),
			GetMutableDefault<UCloudSavedGameSettings>()
		);
	}
#endif
}

void FCloudSavedGameEditorModule::UnregisterSettings()
{
#if WITH_EDITOR
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "CloudSavedGame");
	}
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCloudSavedGameEditorModule, CloudSavedGameEditor)
