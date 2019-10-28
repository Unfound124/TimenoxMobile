// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "Interfaces/SavedGameStorage.h"
#include "Android/AndroidJNI.h"

namespace gpg
{
	class GameServices;
};

/**
 * GooglePlay SnapshotManager implementation
 */
class FAndroidSavedGameStorage : public ISavedGameStorage
{
public:
	FAndroidSavedGameStorage();
	virtual ~FAndroidSavedGameStorage() {}

	/**
	 * Save Game to GooglePlay
	 * Available Policies: CSGP_MOST_RECENTLY_MODIFIED, CSGP_MANUAL, CSGP_HIGHEST_PROGRESS, CSGP_LONGEST_PLAYTIME
	 */
	virtual void SaveGame(const FString& SlotName, USaveGame* SaveGame, ECloudSavedGamePolicy Policy, USavedGameAsyncSaveGame* AsyncAction) override;

	/**
	 * Load Game from GooglePlay
	 * Available Policies: CSGP_MOST_RECENTLY_MODIFIED, CSGP_MANUAL, CSGP_HIGHEST_PROGRESS, CSGP_LONGEST_PLAYTIME
	 */
	virtual void LoadGame(const FString& SlotName, ECloudSavedGamePolicy Policy, bool bCheckInternetConnection, USavedGameAsyncLoadGame* AsyncAction) override;

	/* Delete Game from GooglePlay */
	virtual void DeleteGame(const FString& SlotName, USavedGameAsyncDeleteGame* AsyncAction) override;

	/* Fetch all metadatas from GooglePlay */
	virtual void FetchAll(USavedGameAsyncFetchAll* AsyncAction) override;

	/* Show Native Select UI */
	virtual void ShowSelectUI(const FString& Title, bool bAllowCreate, bool bAllowDelete, int32 MaxSnapshots, USavedGameAsyncShowExternalUI* AsyncAction) override;

	/**
	* Manual Resolve Conflict
	* Synonim for SaveGame with CSGP_MANUAL Policy
	*/
	virtual void ResolveConflict(const FString& SlotName, USaveGame* SaveGame, USavedGameAsyncSaveGame* AsyncAction) override;

	/**
	 * Get GooglePlay Game Services
	 */
	gpg::GameServices* GetGameServices();

private:
	static jmethodID HasInternetConnectionMethod_;
	bool bDoNotUseResolveConflict_;
};