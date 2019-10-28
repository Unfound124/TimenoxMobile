// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SavedGameStorage.h"

/**
 * Module Interface with Cloud Storage access (for Platform modules)
 */
class CLOUDSAVEDGAMECORE_API ISavedGameModule : public IModuleInterface
{
public:
	/* Get Platform Cloud Storage */
	virtual ISavedGameStoragePtr GetCloudStorage() const { return nullptr; }
};