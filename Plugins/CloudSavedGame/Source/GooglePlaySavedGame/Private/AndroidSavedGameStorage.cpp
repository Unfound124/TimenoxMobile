// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#include "AndroidSavedGameStorage.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SavedGameAsyncAction.h"
#include "SystemUtilities.h"
#include <string>
#include <chrono>
#include "GooglePlayCloudSaveGame.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemGooglePlay.h"
#include "Android/AndroidApplication.h"
#include "Misc/ConfigCacheIni.h"

THIRD_PARTY_INCLUDES_START
#include "gpg/snapshot_manager.h"
#include "gpg/snapshot_metadata_change_builder.h"
THIRD_PARTY_INCLUDES_END

gpg::SnapshotConflictPolicy GetNativePolicy(ECloudSavedGamePolicy Policy)
{
	switch (Policy) {
	case ECloudSavedGamePolicy::CSGP_MOST_RECENTLY_MODIFIED:
		return gpg::SnapshotConflictPolicy::MOST_RECENTLY_MODIFIED;
	case ECloudSavedGamePolicy::CSGP_HIGHEST_PROGRESS:
		return gpg::SnapshotConflictPolicy::HIGHEST_PROGRESS;
	case ECloudSavedGamePolicy::CSGP_LONGEST_PLAYTIME:
		return gpg::SnapshotConflictPolicy::LONGEST_PLAYTIME;
	case ECloudSavedGamePolicy::CSGP_MANUAL:
		return gpg::SnapshotConflictPolicy::MANUAL;
	}

	return gpg::SnapshotConflictPolicy::LAST_KNOWN_GOOD;
}

jmethodID FAndroidSavedGameStorage::HasInternetConnectionMethod_;

FAndroidSavedGameStorage::FAndroidSavedGameStorage() :
	bDoNotUseResolveConflict_(false)
{
	if (GConfig) {
		GConfig->GetBool(
			TEXT("/Script/CloudSavedGameEditor.CloudSavedGameSettings"),
			TEXT("bAndroidDoNotUseResolveConflict"),
			bDoNotUseResolveConflict_,
			GEngineIni
		);
	}

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		HasInternetConnectionMethod_ = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_HasInternetConnection", "()Z", false);
	}
}

gpg::GameServices* FAndroidSavedGameStorage::GetGameServices()
{
	FOnlineSubsystemGooglePlay* onlineSubsystem = (FOnlineSubsystemGooglePlay*)FOnlineSubsystemGooglePlay::Get(GOOGLEPLAY_SUBSYSTEM);
	if (!onlineSubsystem) return nullptr;

	return onlineSubsystem->GetGameServices();
}

void FAndroidSavedGameStorage::ShowSelectUI(const FString & Title, bool bAllowCreate, bool bAllowDelete, int32 MaxSnapshots, USavedGameAsyncShowExternalUI * AsyncAction)
{
	gpg::GameServices* gameServices = GetGameServices();
	if (!gameServices) {
		UE_LOG(LogCloudSavedGame, Error, TEXT("GooglePlay GameServices is not initialized in method 'ShowSelectUI'"));
		AsyncAction->OnFailure.Broadcast();
		return;
	}

	gameServices->Snapshots().ShowSelectUIOperation(
		bAllowCreate,
		bAllowDelete,
		MaxSnapshots,
		TCHAR_TO_UTF8(*Title),
		[this, AsyncAction](gpg::SnapshotManager::SnapshotSelectUIResponse const &
			UIresponse) {
		if (gpg::IsSuccess(UIresponse.status)) {
			AsyncAction->OnSelect.Broadcast(UTF8_TO_TCHAR(UIresponse.data.FileName().c_str()));
			return;
		}

		if (UIresponse.status == gpg::UIStatus::ERROR_CANCELED) {
			AsyncAction->OnCancel.Broadcast();
			return;
		}

		AsyncAction->OnFailure.Broadcast();
	});
}

void FAndroidSavedGameStorage::SaveGame(const FString& SlotName, USaveGame* SaveGame, ECloudSavedGamePolicy Policy, USavedGameAsyncSaveGame* AsyncAction)
{
	gpg::GameServices* gameServices = GetGameServices();
	if (!gameServices) {
		UE_LOG(LogCloudSavedGame, Error, TEXT("GooglePlay GameServices is not initialized in method 'SaveGame'"));
		AsyncAction->OnFailure.Broadcast();
		return;
	}

	gpg::SnapshotConflictPolicy ConflictPolicy = GetNativePolicy(Policy);
	gameServices->Snapshots().Open(
		TCHAR_TO_UTF8(*SlotName),
		ConflictPolicy,
		[this, gameServices, SaveGame, AsyncAction](gpg::SnapshotManager::OpenResponse const &
			OpenResponse) {
		if (gpg::IsSuccess(OpenResponse.status) && OpenResponse.conflict_id.empty()) {
			// Default metadada value
			FString description = TCHAR_TO_UTF8(*SaveGame->GetClass()->GetName());
			gpg::Duration playedTime;
			int64_t progressValue = 0;

			// Fill metadata from SaveGame if can
			UGooglePlayCloudSaveGame* googlePlaySaveGame = Cast<UGooglePlayCloudSaveGame>(SaveGame);
			if (googlePlaySaveGame) {
				description = googlePlaySaveGame->GetDescription();
				progressValue = googlePlaySaveGame->GetProgressValue();

				std::chrono::duration<float> Duration(googlePlaySaveGame->GetPlayedTime());
				playedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Duration);
			}

			gpg::SnapshotMetadataChange metadataChange = gpg::SnapshotMetadataChange::Builder()
				.SetDescription(TCHAR_TO_UTF8(*description))
				.SetPlayedTime(playedTime)
				.SetProgressValue(progressValue)
				.Create();

			TArray<uint8> objectBytes;
			std::vector<uint8> writeData;
			UGameplayStatics::SaveGameToMemory(SaveGame, objectBytes);
			USystemUtilities::TArrayToVector(objectBytes, writeData);

			gpg::SnapshotManager::CommitResponse WriteResponse =
				gameServices->Snapshots().CommitBlocking(OpenResponse.data, metadataChange, writeData);

			if (gpg::IsSuccess(WriteResponse.status)) {
				AsyncAction->OnSuccess.Broadcast();
				return;
			}

			UE_LOG(LogCloudSavedGame, Error, TEXT("Saving Data to GooglePlay failed on Commit with code: %i"), (int)WriteResponse.status);
		}
		else {
			UE_LOG(LogCloudSavedGame, Error, TEXT("Saving Data to GooglePlay failed with code: %i, conflict id: %s"), (int)OpenResponse.status, UTF8_TO_TCHAR(OpenResponse.conflict_id.c_str()));
		}

		AsyncAction->OnFailure.Broadcast();
	});
}

void FAndroidSavedGameStorage::LoadGame(const FString & SlotName, ECloudSavedGamePolicy Policy, bool bCheckInternetConnection, USavedGameAsyncLoadGame * AsyncAction)
{
	/* Check internet connection */
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	if (bCheckInternetConnection && Env)
	{
		bool bResult = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FAndroidSavedGameStorage::HasInternetConnectionMethod_);
		if (!bResult) {
			UE_LOG(LogCloudSavedGame, Warning, TEXT("LoadGame can be performed only with internet connection"));
			AsyncAction->OnFailure.Broadcast(FSavedGameMetadata(), nullptr);
			return;
		}
	}
	/* ~Check internet connection */

	gpg::GameServices* gameServices = GetGameServices();
	if (!gameServices) {
		UE_LOG(LogCloudSavedGame, Error, TEXT("GooglePlay GameServices is not initialized in method 'LoadGame'"));
		AsyncAction->OnFailure.Broadcast(FSavedGameMetadata(), nullptr);
		return;
	}

	gpg::SnapshotConflictPolicy ConflictPolicy = GetNativePolicy(Policy);
	gameServices->Snapshots().Open(
		TCHAR_TO_UTF8(*SlotName),
		ConflictPolicy,
		[this, gameServices, AsyncAction](gpg::SnapshotManager::OpenResponse const &
			OpenResponse) {
		if (gpg::IsSuccess(OpenResponse.status) && OpenResponse.conflict_id.empty()) {
			gpg::SnapshotManager::ReadResponse ReadData = gameServices->Snapshots().ReadBlocking(OpenResponse.data);
			if (gpg::IsSuccess(ReadData.status)) {
				FSavedGameMetadata metadata(
					UTF8_TO_TCHAR(OpenResponse.data.FileName().c_str()),
					FDateTime::FromUnixTimestamp(OpenResponse.data.LastModifiedTime().count() / 1000),
					false, // owning device is always false
					OpenResponse.data.PlayedTime().count() / 1000.f,
					OpenResponse.data.ProgressValue(),
					UTF8_TO_TCHAR(OpenResponse.data.Description().c_str())
				);

				TArray<uint8> bytesArray(ReadData.data.data(), ReadData.data.size());
				USaveGame* resultSaveGame = USystemUtilities::SaveGameFromMemory(bytesArray);

				if (resultSaveGame) {
					UGooglePlayCloudSaveGame* gpSaveGame = Cast<UGooglePlayCloudSaveGame>(resultSaveGame);
					if (gpSaveGame) {
						gpSaveGame->SetDescription(metadata.Description);
						gpSaveGame->SetPlayedTime(metadata.PlayedTime);
						gpSaveGame->SetProgressValue(metadata.ProgressValue);
					}

					AsyncAction->OnSuccess.Broadcast(metadata, resultSaveGame);
					return;
				}

				UE_LOG(LogCloudSavedGame, Error, TEXT("Cannot convert readed data to USaveGame, filename: %s"), UTF8_TO_TCHAR(OpenResponse.data.FileName().c_str()));
				AsyncAction->OnFailure.Broadcast(metadata, nullptr);
				return;
			}
			else {
				UE_LOG(LogCloudSavedGame, Error, TEXT("Reading Save Data failed with code: %i, filename: %s"), (int)ReadData.status, UTF8_TO_TCHAR(OpenResponse.data.FileName().c_str()));
			}
		}
		else {
			UE_LOG(LogCloudSavedGame, Error, TEXT("Loading Save Data failed with code: %i, conflict id: %s"), (int)OpenResponse.status, UTF8_TO_TCHAR(OpenResponse.conflict_id.c_str()));
		}

		AsyncAction->OnFailure.Broadcast(FSavedGameMetadata(), nullptr);
	});
}

void FAndroidSavedGameStorage::DeleteGame(const FString & SlotName, USavedGameAsyncDeleteGame * AsyncAction)
{
	gpg::GameServices* gameServices = GetGameServices();
	if (!gameServices) {
		UE_LOG(LogCloudSavedGame, Error, TEXT("GooglePlay GameServices is not initialized in method 'DeleteGame'"));
		AsyncAction->OnFailure.Broadcast();
		return;
	}

	gameServices->Snapshots().Open(
		TCHAR_TO_UTF8(*SlotName),
		gpg::SnapshotConflictPolicy::MANUAL,
		[this, gameServices, AsyncAction](gpg::SnapshotManager::OpenResponse const &
			OpenResponse) {
		if (gpg::IsSuccess(OpenResponse.status) && OpenResponse.conflict_id.empty()) {
			gameServices->Snapshots().Delete(OpenResponse.data);
			AsyncAction->OnSuccess.Broadcast();
			return;
		}

		UE_LOG(LogCloudSavedGame, Error, TEXT("Deleting Save Data failed with code: %i, conflict id: %s"), (int)OpenResponse.status, UTF8_TO_TCHAR(OpenResponse.conflict_id.c_str()));
		AsyncAction->OnFailure.Broadcast();
	});
}

void FAndroidSavedGameStorage::FetchAll(USavedGameAsyncFetchAll * AsyncAction)
{
	gpg::GameServices* gameServices = GetGameServices();
	if (!gameServices) {
		UE_LOG(LogCloudSavedGame, Error, TEXT("GooglePlay GameServices is not initialized in method 'FetchAll'"));
		TArray<FSavedGameMetadata> resultArray;
		AsyncAction->OnFailure.Broadcast(resultArray);
		return;
	}

	gameServices->Snapshots().FetchAll(
		[this, AsyncAction](gpg::SnapshotManager::FetchAllResponse const &
			FetchAllResponse) {
		TArray<FSavedGameMetadata> *resultArray = new TArray<FSavedGameMetadata>();

		if (gpg::IsSuccess(FetchAllResponse.status)) {
			for (auto it : FetchAllResponse.data) {
				resultArray->Add(FSavedGameMetadata(
					UTF8_TO_TCHAR(it.FileName().c_str()),
					FDateTime::FromUnixTimestamp(it.LastModifiedTime().count() / 1000),
					false, // owning device is always false
					it.PlayedTime().count() / 1000.f,
					it.ProgressValue()
				));
			}

			if (resultArray->Num() > 0) {
				AsyncAction->OnSuccess.Broadcast(*resultArray);
				return;
			}
		}

		UE_LOG(LogCloudSavedGame, Error, TEXT("Fetching All Saves failed with code: %i"), (int)FetchAllResponse.status);
		AsyncAction->OnFailure.Broadcast(*resultArray);
	});
}

void FAndroidSavedGameStorage::ResolveConflict(const FString& SlotName, USaveGame* SaveGame, USavedGameAsyncSaveGame* AsyncAction)
{
	UE_LOG(LogCloudSavedGame, Warning, TEXT("ResolveConflict on Android is always success"));

	if (bDoNotUseResolveConflict_) {
		AsyncAction->OnSuccess.Broadcast();
		return;
	}

	this->SaveGame(SlotName, SaveGame, ECloudSavedGamePolicy::CSGP_MANUAL, AsyncAction);
}