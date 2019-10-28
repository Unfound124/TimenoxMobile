// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SavedGameMetadata.h"
#include "CloudSavedGameTypes.h"
#include "SavedGameAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCloudSaveGameSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCloudSaveGameSelectSignature, const FString&, SlotName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCloudLoadGameSignature, FSavedGameMetadata, Metadata, USaveGame*, SaveGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCloudSavedGameFetchAllCallback, const TArray<FSavedGameMetadata>&, SavedGamesMetadata);

/* SaveGame Method type */
enum class ESaveGameMethod : uint8
{
	SGM_SAVEGAME,
	SGM_RESOLVECONFLICT
};

/**
 * Async Action Class for Save Game and Resolve Conflict
 */
UCLASS()
class CLOUDSAVEDGAMECORE_API USavedGameAsyncSaveGame : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCloudSaveGameSignature OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FCloudSaveGameSignature OnFailure;

	/* Save Game to Cloud */
	UFUNCTION(BlueprintCallable, Category = CloudSavedGame, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Save Game to Cloud"))
	static USavedGameAsyncSaveGame* SaveGame(UObject* WorldContextObject, const FString& SlotName, USaveGame* SaveGame, ECloudSavedGamePolicy Policy = ECloudSavedGamePolicy::CSGP_MOST_RECENTLY_MODIFIED);

	/* Resolve Conflict of SavedGame in Cloud */
	UFUNCTION(BlueprintCallable, Category = CloudSavedGame, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Resolve Conflict on Cloud"))
	static USavedGameAsyncSaveGame* ResolveConflict(UObject* WorldContextObject, const FString& SlotName, USaveGame* SaveGame);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~UBlueprintAsyncActionBase interface

protected:
	UObject* worldContextObject_;    // World Context, should be not null
	FString slotName_;
	USaveGame* saveGame_; // SavedGame instance
	ECloudSavedGamePolicy policy_;
	ESaveGameMethod saveMethod_;
};

/**
* Async Action Class for Load Game
*/
UCLASS()
class CLOUDSAVEDGAMECORE_API USavedGameAsyncLoadGame : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCloudLoadGameSignature OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FCloudLoadGameSignature OnFailure;

	/** Load Game from Cloud
	 * CheckInternetConnection prevents load from cache
	 */
	UFUNCTION(BlueprintCallable, Category = CloudSavedGame, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Load Game from Cloud"))
	static USavedGameAsyncLoadGame* LoadGame(UObject* WorldContextObject, const FString& SlotName, ECloudSavedGamePolicy Policy = ECloudSavedGamePolicy::CSGP_MOST_RECENTLY_MODIFIED, bool bCheckInternetConnection = false);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~UBlueprintAsyncActionBase interface

protected:
	UObject * worldContextObject_;    // World Context, should be not null
	FString slotName_;
	ECloudSavedGamePolicy policy_;
	bool bCheckInternetConnection_; // flag to check connection before reading
};

/**
* Async Action Class for Delete Game
*/
UCLASS()
class CLOUDSAVEDGAMECORE_API USavedGameAsyncDeleteGame : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCloudSaveGameSignature OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FCloudSaveGameSignature OnFailure;

	/* Delete Game from Cloud */
	UFUNCTION(BlueprintCallable, Category = CloudSavedGame, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Delete Game from Cloud"))
	static USavedGameAsyncDeleteGame* DeleteGame(UObject* WorldContextObject, const FString& SlotName);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~UBlueprintAsyncActionBase interface

protected:
	UObject * worldContextObject_;    // World Context, should be not null
	FString slotName_;
};

/**
* Async Action Class for Fetch All
*/
UCLASS()
class CLOUDSAVEDGAMECORE_API USavedGameAsyncFetchAll : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCloudSavedGameFetchAllCallback OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FCloudSavedGameFetchAllCallback OnFailure;

	/* Fetch all metadatas from Cloud */
	UFUNCTION(BlueprintCallable, Category = CloudSavedGame, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Fetch All Games from Cloud"))
	static USavedGameAsyncFetchAll* FetchAll(UObject* WorldContextObject);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~UBlueprintAsyncActionBase interface

protected:
	UObject * worldContextObject_;    // World Context, should be not null
};

/**
* Async Action Class for Show Native External UI
*/
UCLASS()
class CLOUDSAVEDGAMECORE_API USavedGameAsyncShowExternalUI : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCloudSaveGameSelectSignature OnSelect;

	UPROPERTY(BlueprintAssignable)
	FCloudSaveGameSignature OnFailure;

	UPROPERTY(BlueprintAssignable)
	FCloudSaveGameSignature OnCancel;

	/**
	 * Show Native External UI for platform
	 * @note: Currently work only for GooglePlay (Android)
	 */
	UFUNCTION(BlueprintCallable, Category = CloudSavedGame, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Show External Select UI"))
	static USavedGameAsyncShowExternalUI* ShowExternalSelectUI(UObject* WorldContextObject, const FString& Title, bool bAllowCreate = true, bool bAllowDelete = true, int32 MaxSnapshots = 5);

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~UBlueprintAsyncActionBase interface

protected:
	UObject* worldContextObject_;    // World Context, should be not null
	FString title_;
	bool bAllowCreate_;
	bool bAllowDelete_;
	int32 maxSnapshots_;
};