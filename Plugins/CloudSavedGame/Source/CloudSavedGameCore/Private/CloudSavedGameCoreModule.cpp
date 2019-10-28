// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#include "CloudSavedGameCoreModule.h"

#define LOCTEXT_NAMESPACE "FCloudSavedGameCoreModule"

FCloudSavedGameCoreModule & FCloudSavedGameCoreModule::Get()
{
	return FModuleManager::LoadModuleChecked<FCloudSavedGameCoreModule>("CloudSavedGameCore");
}

void FCloudSavedGameCoreModule::StartupModule()
{
}

void FCloudSavedGameCoreModule::ShutdownModule()
{
}

ISavedGameStoragePtr FCloudSavedGameCoreModule::GetCloudStorage() const
{
#if PLATFORM_IOS
	return FModuleManager::LoadModuleChecked<ISavedGameModule>("ICloudSavedGame").GetCloudStorage();
#elif PLATFORM_ANDROID
	return FModuleManager::LoadModuleChecked<ISavedGameModule>("GooglePlaySavedGame").GetCloudStorage();
#else
	return nullptr;
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCloudSavedGameCoreModule, CloudSavedGameCore)
