// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#include "SavedGameAsyncAction.h"
#include "CloudSavedGameCoreModule.h"
#include "SavedGameAsyncAction.h"
#include "Kismet/GameplayStatics.h"

USavedGameAsyncSaveGame* USavedGameAsyncSaveGame::SaveGame(UObject* WorldContextObject, const FString& SlotName, USaveGame* SaveGame, ECloudSavedGamePolicy Policy)
{
	if (!WorldContextObject || SlotName.IsEmpty() || !SaveGame)
		return nullptr;

	USavedGameAsyncSaveGame* asyncFunction = NewObject<USavedGameAsyncSaveGame>();
	asyncFunction->worldContextObject_ = WorldContextObject;
	asyncFunction->slotName_ = SlotName;
	asyncFunction->saveGame_ = SaveGame;
	asyncFunction->policy_ = Policy;
	asyncFunction->saveMethod_ = ESaveGameMethod::SGM_SAVEGAME;

	return asyncFunction;
}

USavedGameAsyncSaveGame* USavedGameAsyncSaveGame::ResolveConflict(UObject * WorldContextObject, const FString & SlotName, USaveGame * SaveGame)
{
	if (!WorldContextObject || SlotName.IsEmpty() || !SaveGame)
		return nullptr;

	USavedGameAsyncSaveGame* asyncFunction = NewObject<USavedGameAsyncSaveGame>();
	asyncFunction->worldContextObject_ = WorldContextObject;
	asyncFunction->slotName_ = SlotName;
	asyncFunction->saveGame_ = SaveGame;
	asyncFunction->saveMethod_ = ESaveGameMethod::SGM_RESOLVECONFLICT;

	return asyncFunction;
}

void USavedGameAsyncSaveGame::Activate()
{
	FCloudSavedGameCoreModule storageModule = FCloudSavedGameCoreModule::Get();
	ISavedGameStoragePtr storageInterface = storageModule.GetCloudStorage();
	if (!storageInterface.IsValid()) {
		UE_LOG(LogCloudSavedGame, Warning, TEXT("Saving Game to Cloud not implemented for platform: %s"), *UGameplayStatics::GetPlatformName());
		OnFailure.Broadcast();
		return;
	}

	switch (saveMethod_) {	
	case ESaveGameMethod::SGM_RESOLVECONFLICT:
		return storageInterface->ResolveConflict(slotName_, saveGame_, this);
	case ESaveGameMethod::SGM_SAVEGAME:
	default:
		return storageInterface->SaveGame(slotName_, saveGame_, policy_, this);
	}
}

USavedGameAsyncLoadGame * USavedGameAsyncLoadGame::LoadGame(UObject * WorldContextObject, const FString& SlotName, ECloudSavedGamePolicy Policy, bool bCheckInternetConnection)
{
	if (!WorldContextObject || SlotName.IsEmpty())
		return nullptr;

	USavedGameAsyncLoadGame* asyncFunction = NewObject<USavedGameAsyncLoadGame>();
	asyncFunction->worldContextObject_ = WorldContextObject;
	asyncFunction->slotName_ = SlotName;
	asyncFunction->policy_ = Policy;
	asyncFunction->bCheckInternetConnection_ = bCheckInternetConnection;

	return asyncFunction;
}

void USavedGameAsyncLoadGame::Activate()
{
	FCloudSavedGameCoreModule storageModule = FCloudSavedGameCoreModule::Get();
	ISavedGameStoragePtr storageInterface = storageModule.GetCloudStorage();
	if (!storageInterface.IsValid()) {
		UE_LOG(LogCloudSavedGame, Warning, TEXT("Loading Game from Cloud not implemented for platform: %s"), *UGameplayStatics::GetPlatformName());
		OnFailure.Broadcast(FSavedGameMetadata(), nullptr);
		return;
	}

	return storageInterface->LoadGame(slotName_, policy_, bCheckInternetConnection_, this);
}

USavedGameAsyncDeleteGame * USavedGameAsyncDeleteGame::DeleteGame(UObject * WorldContextObject, const FString& SlotName)
{
	if (!WorldContextObject || SlotName.IsEmpty())
		return nullptr;

	USavedGameAsyncDeleteGame* asyncFunction = NewObject<USavedGameAsyncDeleteGame>();
	asyncFunction->worldContextObject_ = WorldContextObject;
	asyncFunction->slotName_ = SlotName;

	return asyncFunction;
}

void USavedGameAsyncDeleteGame::Activate()
{
	FCloudSavedGameCoreModule storageModule = FCloudSavedGameCoreModule::Get();
	ISavedGameStoragePtr storageInterface = storageModule.GetCloudStorage();
	if (!storageInterface.IsValid()) {
		UE_LOG(LogCloudSavedGame, Warning, TEXT("Deleting Game from Cloud not implemented for platform: %s"), *UGameplayStatics::GetPlatformName());
		OnFailure.Broadcast();
		return;
	}

	return storageInterface->DeleteGame(slotName_, this);
}

USavedGameAsyncFetchAll * USavedGameAsyncFetchAll::FetchAll(UObject * WorldContextObject)
{
	if (!WorldContextObject)
		return nullptr;

	USavedGameAsyncFetchAll* asyncFunction = NewObject<USavedGameAsyncFetchAll>();
	asyncFunction->worldContextObject_ = WorldContextObject;

	return asyncFunction;
}

void USavedGameAsyncFetchAll::Activate()
{
	FCloudSavedGameCoreModule storageModule = FCloudSavedGameCoreModule::Get();
	ISavedGameStoragePtr storageInterface = storageModule.GetCloudStorage();
	if (!storageInterface.IsValid()) {
		UE_LOG(LogCloudSavedGame, Warning, TEXT("Fetching saves list from Cloud not implemented for platform: %s"), *UGameplayStatics::GetPlatformName());
		TArray<FSavedGameMetadata> blankArray;
		OnFailure.Broadcast(blankArray);
		return;
	}

	return storageInterface->FetchAll(this);
}

USavedGameAsyncShowExternalUI * USavedGameAsyncShowExternalUI::ShowExternalSelectUI(UObject* WorldContextObject, const FString& Title, bool bAllowCreate, bool bAllowDelete, int32 MaxSnapshots)
{
	if (!WorldContextObject)
		return nullptr;

	USavedGameAsyncShowExternalUI* asyncFunction = NewObject<USavedGameAsyncShowExternalUI>();
	asyncFunction->worldContextObject_ = WorldContextObject;
	asyncFunction->bAllowCreate_ = bAllowCreate;
	asyncFunction->bAllowDelete_ = bAllowDelete;
	asyncFunction->maxSnapshots_ = MaxSnapshots;

	if (!Title.IsEmpty())
		asyncFunction->title_ = Title;

	return asyncFunction;
}

void USavedGameAsyncShowExternalUI::Activate()
{
	FCloudSavedGameCoreModule storageModule = FCloudSavedGameCoreModule::Get();
	ISavedGameStoragePtr storageInterface = storageModule.GetCloudStorage();
	if (!storageInterface.IsValid()) {
		UE_LOG(LogCloudSavedGame, Warning, TEXT("ShowExternalUI not implemented for platform: %s"), *UGameplayStatics::GetPlatformName());
		OnFailure.Broadcast();
		return;
	}

	return storageInterface->ShowSelectUI(title_, bAllowCreate_, bAllowDelete_, maxSnapshots_, this);
}