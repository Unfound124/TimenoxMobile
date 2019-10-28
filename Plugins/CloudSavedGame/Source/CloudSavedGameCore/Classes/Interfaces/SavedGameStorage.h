// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CloudSavedGameTypes.h"

class USaveGame;
class USavedGameAsyncSaveGame;
class USavedGameAsyncLoadGame;
class USavedGameAsyncDeleteGame;
class USavedGameAsyncFetchAll;
class USavedGameAsyncShowExternalUI;
class ISavedGameStorage;

typedef TSharedPtr<ISavedGameStorage, ESPMode::ThreadSafe> ISavedGameStoragePtr;

/**
 * Basic Interface for Cloud Storage with Saved game implementation
 */
class CLOUDSAVEDGAMECORE_API ISavedGameStorage
{
public:
	ISavedGameStorage() {}
	virtual ~ISavedGameStorage() {}
	
	/**
	 * SaveGame to Cloud
	 * @param SlotName - filename in Cloud
	 * @param SaveGame - Game data
	 * @param Policy - Write to Cloud policy
	 * @param AsyncAction - function caller/instigator
	 */
	virtual void SaveGame(const FString& SlotName, USaveGame* SaveGame, ECloudSavedGamePolicy Policy, USavedGameAsyncSaveGame* AsyncAction) = 0;
	
	/**
	* LoadGame from Cloud
	* @param SlotName - filename in Cloud
	* @param Policy - Read from Cloud policy
	* @param AsyncAction - function caller/instigator
	* @param bCheckInternetConnection - load game if has open internet connection
	*/
	virtual void LoadGame(const FString& SlotName, ECloudSavedGamePolicy Policy, bool bCheckInternetConnection, USavedGameAsyncLoadGame* AsyncAction) = 0;
	
	/**
	* Delete from Cloud
	* @param SlotName - filename in Cloud
	* @param AsyncAction - function caller/instigator
	*/
	virtual void DeleteGame(const FString& SlotName, USavedGameAsyncDeleteGame* AsyncAction) = 0;
	
	/**
	* Fetch all metadatas from Cloud
	* @param AsyncAction - function caller/instigator
	*/
	virtual void FetchAll(USavedGameAsyncFetchAll* AsyncAction) = 0;
	
	/**
	* Show native Select UI
	* @param Title - window title
	* @param bAllowCreate - Allow to create via UI
	* @param bAllowDelete - Allow to delete via UI
	* @param MaxSnapshots - number of snapshots in list
	* @param AsyncAction - function caller/instigator
	*/
	virtual void ShowSelectUI(const FString& Title, bool bAllowCreate, bool bAllowDelete, int32 MaxSnapshots, USavedGameAsyncShowExternalUI* AsyncAction) = 0;
	
	/**
	* Manual Resolve Conflict (Force Save)
	* @param SlotName - filename in Cloud
	* @param SaveGame - Game data
	* @param AsyncAction - function caller/instigator
	*/
	virtual void ResolveConflict(const FString& SlotName, USaveGame* SaveGame, USavedGameAsyncSaveGame* AsyncAction) = 0;
};