// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "Interfaces/SavedGameModule.h"

/**
 * GooglePlay Cloud Module
 */
class GOOGLEPLAYSAVEDGAME_API FGooglePlaySavedGameModule : public ISavedGameModule
{
public:
	// Load and get module
	static FGooglePlaySavedGameModule& Get();

	/* IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/* ~IModuleInterface implementation */

	/* ISavedGameModule module */
	inline virtual ISavedGameStoragePtr GetCloudStorage() const override {
		return cloudStorageImpl_; 
	}
	/* ~ISavedGameModule module */

private:
	ISavedGameStoragePtr cloudStorageImpl_; // GooglePlay SnapshotManager implementation
};