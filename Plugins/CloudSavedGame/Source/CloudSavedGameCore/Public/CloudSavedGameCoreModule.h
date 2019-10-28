// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "Interfaces/SavedGameModule.h"

/**
 * CloudSavedGame Module class
 */
class FCloudSavedGameCoreModule : public ISavedGameModule
{
public:
	// Load and get module
	static FCloudSavedGameCoreModule& Get();

	/* IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/* ~IModuleInterface implementation */

	// Get current storage implementation
	virtual ISavedGameStoragePtr GetCloudStorage() const;
};