// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#include "ICloudSavedGameModule.h"
#include "IOSSavedGameStorage.h"

#define LOCTEXT_NAMESPACE "FICloudSavedGameModule"

FICloudSavedGameModule & FICloudSavedGameModule::Get()
{
	return FModuleManager::LoadModuleChecked<FICloudSavedGameModule>("ICloudSavedGame");
}

void FICloudSavedGameModule::StartupModule()
{
	cloudStorageImpl_ = MakeShareable(new FIOSSavedGameStorage());
}

void FICloudSavedGameModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FICloudSavedGameModule, ICloudSavedGame)
