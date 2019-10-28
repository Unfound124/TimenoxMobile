// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "Interfaces/SavedGameStorage.h"
#import <GameKit/GameKit.h>

/**
 * iCloud GKSavedGame implementation
 */
class FIOSSavedGameStorage : public ISavedGameStorage
{
public:
	FIOSSavedGameStorage();
	virtual ~FIOSSavedGameStorage() {}

	/**
	 * Save Game to iCloud
	 * Policy is not used for iCloud
	 */
	virtual void SaveGame(const FString& SlotName, USaveGame* SaveGame, ECloudSavedGamePolicy Policy, USavedGameAsyncSaveGame* AsyncAction) override;

	/**
	 * Load Game from iCloud
	 * Available policies: CSGP_MOST_RECENTLY_MODIFIED, CSGP_OWNING_DEVICE
	 */
	virtual void LoadGame(const FString& SlotName, ECloudSavedGamePolicy Policy, bool bCheckInternetConnection, USavedGameAsyncLoadGame* AsyncAction) override;

	/* Delete Game from iCloud */
	virtual void DeleteGame(const FString& SlotName, USavedGameAsyncDeleteGame* AsyncAction) override;

	/* Fetch all metadatas from iCloud */
	virtual void FetchAll(USavedGameAsyncFetchAll* AsyncAction) override;

	/* Show Native Select UI is not implemented for iCloud */
	virtual void ShowSelectUI(const FString& Title, bool bAllowCreate, bool bAllowDelete, int32 MaxSnapshots, USavedGameAsyncShowExternalUI* AsyncAction) override;

	/**
	 * Manual Resolve Conflict
	 * Force Use selected SaveGame and Delete all other duplicates
	 */
	virtual void ResolveConflict(const FString& SlotName, USaveGame* SaveGame, USavedGameAsyncSaveGame* AsyncAction) override;

private:
	bool HasInternetConnection();
};