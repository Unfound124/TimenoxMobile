// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#include "GooglePlaySavedGameModule.h"
#include "AndroidSavedGameStorage.h"

#define LOCTEXT_NAMESPACE "FGooglePlaySavedGameModule"

FGooglePlaySavedGameModule & FGooglePlaySavedGameModule::Get()
{
	return FModuleManager::LoadModuleChecked<FGooglePlaySavedGameModule>("GooglePlaySavedGames");
}

void FGooglePlaySavedGameModule::StartupModule()
{
	cloudStorageImpl_ = MakeShareable(new FAndroidSavedGameStorage());
}

void FGooglePlaySavedGameModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGooglePlaySavedGameModule, GooglePlaySavedGame)
