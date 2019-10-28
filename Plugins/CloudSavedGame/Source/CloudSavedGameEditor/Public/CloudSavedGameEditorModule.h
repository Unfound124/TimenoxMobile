// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * CloudSavedGame Module class
 */
class FCloudSavedGameEditorModule : public IModuleInterface
{
public:
	/* IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	/* ~IModuleInterface implementation */

private:
	/* Project Settings */
	void RegisterSettings();
	void UnregisterSettings();
	/* ~Project Settings */
};