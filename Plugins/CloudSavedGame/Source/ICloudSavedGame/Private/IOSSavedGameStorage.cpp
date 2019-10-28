// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#include "IOSSavedGameStorage.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SavedGameAsyncAction.h"
#include "SystemUtilities.h"
#include <string>
#include <algorithm>
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>


// Sort by Policies
struct FSavedGameSorter
{
	// CSGP_OWNING_DEVICE
	static bool SortByOwningDevice(const GKSavedGame* A, const GKSavedGame* B) 
	{
		return[A deviceName] == [[UIDevice currentDevice] name];
	}

	// CSGP_MOST_RECENTLY_MODIFIED
	static bool SortByModificationDate(const GKSavedGame* A, const GKSavedGame* B)
	{
		return [A modificationDate] > [B modificationDate];
	}
};

FIOSSavedGameStorage::FIOSSavedGameStorage()
{
}

void FIOSSavedGameStorage::SaveGame(const FString& SlotName, USaveGame* SaveGame, ECloudSavedGamePolicy Policy, USavedGameAsyncSaveGame* AsyncAction)
{
	TArray<uint8> objectBytes;
	UGameplayStatics::SaveGameToMemory(SaveGame, objectBytes);

	GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
	[localPlayer saveGameData : [NSData dataWithBytes:objectBytes.GetData() length:objectBytes.Num()]
		withName:[NSString stringWithUTF8String:TCHAR_TO_UTF8(*SlotName)]
		completionHandler:^(GKSavedGame * _Nullable savedGame, NSError * _Nullable error) {
		if (error == nil) {
			UE_LOG(LogCloudSavedGame, Log, TEXT("Saving to iCloud succeeded"));
			AsyncAction->OnSuccess.Broadcast();
		}
		else {
			UE_LOG(LogCloudSavedGame, Error, TEXT("Saving to iCloud failed with code: %i (%s)"), error.code, UTF8_TO_TCHAR([error.description UTF8String]));
			AsyncAction->OnFailure.Broadcast();
		}
	}];
}

void FIOSSavedGameStorage::LoadGame(const FString & SlotName, ECloudSavedGamePolicy Policy, bool bCheckInternetConnection, USavedGameAsyncLoadGame * AsyncAction)
{
	/* Check internet connection */
	if (bCheckInternetConnection && !HasInternetConnection()) {
		UE_LOG(LogCloudSavedGame, Warning, TEXT("LoadGame can be performed only with internet connection"));
		AsyncAction->OnFailure.Broadcast(FSavedGameMetadata(), nullptr);
		return;
	}
	/* ~Check internet connection */

	NSString *slotNameIOS = [NSString stringWithUTF8String:TCHAR_TO_UTF8(*SlotName)];
	GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
	[localPlayer fetchSavedGamesWithCompletionHandler:^(NSArray<GKSavedGame *> * _Nullable savedGames, NSError * _Nullable error) {
		if (error == nil) {
			std::vector<GKSavedGame*> foundSaves;

			for (GKSavedGame* savedGame in savedGames) {
				if ([savedGame.name isEqualToString:slotNameIOS]) {
					foundSaves.push_back(savedGame);
				}
			}

			if (foundSaves.size() == 0) {
				UE_LOG(LogCloudSavedGame, Log, TEXT("Save with name '%s' not found in iCloud"), *SlotName);
				AsyncAction->OnFailure.Broadcast(FSavedGameMetadata(SlotName), nullptr); // Did not found
				return;
			}

			if (foundSaves.size() > 1) {
				UE_LOG(LogCloudSavedGame, Warning, TEXT("Found %i Saved with same name '%s'"), foundSaves.size(), *SlotName);
			}

			/* find by conflict policy */
			switch (Policy) {
			case ECloudSavedGamePolicy::CSGP_OWNING_DEVICE: {
				std::sort(foundSaves.begin(), foundSaves.end(), FSavedGameSorter::SortByOwningDevice);
				break;
			}
			case ECloudSavedGamePolicy::CSGP_MOST_RECENTLY_MODIFIED: {
				std::sort(foundSaves.begin(), foundSaves.end(), FSavedGameSorter::SortByModificationDate);
				break;
			}
			}

			GKSavedGame* saveToLoad = foundSaves[0];
			FSavedGameMetadata metadata(
				SlotName,
				FDateTime::FromUnixTimestamp((int64)[saveToLoad.modificationDate timeIntervalSince1970]),
				[saveToLoad deviceName] == [[UIDevice currentDevice] name]
			);

			[saveToLoad loadDataWithCompletionHandler:^(NSData* _Nullable data, NSError* _Nullable loadError) {
				if (loadError == nil && (data != Nil && [data isKindOfClass:[NSData class]]) && [data length] > 0) {

					const uint8* rawData = (uint8*)[data bytes];
					TArray<uint8> bytesArray(rawData, [data length]);

					UE_LOG(LogCloudSavedGame, Log, TEXT("Loading Data from iCloud with name '%s' succeeded"), *SlotName);
					USaveGame* resultSaveGame = USystemUtilities::SaveGameFromMemory(bytesArray);
					if(resultSaveGame)
						AsyncAction->OnSuccess.Broadcast(metadata, resultSaveGame);
					else
						AsyncAction->OnFailure.Broadcast(metadata, nullptr);
				}
				else {
					UE_LOG(LogCloudSavedGame, Error, TEXT("Loaded Data with slot name '%s' is empty"), *SlotName);
					AsyncAction->OnFailure.Broadcast(metadata, nullptr);
				}
			}];
		}
		else {
			UE_LOG(LogCloudSavedGame, Error, TEXT("Loading from iCloud failed with code: %i (%s)"), error.code, UTF8_TO_TCHAR([error.description UTF8String]));
			AsyncAction->OnFailure.Broadcast(FSavedGameMetadata(), nullptr);
		}
	}];
}

void FIOSSavedGameStorage::DeleteGame(const FString & SlotName, USavedGameAsyncDeleteGame * AsyncAction)
{
	GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
	[localPlayer deleteSavedGamesWithName:[NSString stringWithUTF8String : TCHAR_TO_UTF8(*SlotName)] completionHandler:^(NSError * _Nullable error) {
		if (error == nil) {
			UE_LOG(LogCloudSavedGame, Log, TEXT("Deleting Save from iCloud succeeded"));
			AsyncAction->OnSuccess.Broadcast();
			return;
		}

		UE_LOG(LogCloudSavedGame, Error, TEXT("Deleting Save from iCloud failed with code: %i (%s)"), error.code, UTF8_TO_TCHAR([error.description UTF8String]));
		AsyncAction->OnFailure.Broadcast();
	}];
}

void FIOSSavedGameStorage::FetchAll(USavedGameAsyncFetchAll * AsyncAction)
{
	GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
	[localPlayer fetchSavedGamesWithCompletionHandler : ^ (NSArray<GKSavedGame *> * _Nullable savedGames, NSError * _Nullable error) {
		TArray<FSavedGameMetadata> *resultArray = new TArray<FSavedGameMetadata>();

		if (error == nil) {
			for (GKSavedGame* savedGame in savedGames) {
				resultArray->Add(
					FSavedGameMetadata(
						UTF8_TO_TCHAR([savedGame.name UTF8String]),
						FDateTime::FromUnixTimestamp((int64)[savedGame.modificationDate timeIntervalSince1970]),
						[savedGame deviceName] == [[UIDevice currentDevice] name]
					)
				);
			}

			UE_LOG(LogCloudSavedGame, Log, TEXT("Found %i Saves in iCloud"), resultArray->Num());

			if (resultArray->Num() > 0)
				AsyncAction->OnSuccess.Broadcast(*resultArray);
			else
				AsyncAction->OnFailure.Broadcast(*resultArray);
		}
		else {
			UE_LOG(LogCloudSavedGame, Log, TEXT("Fetching from iCloud failed with code: %i (%s)"), error.code, UTF8_TO_TCHAR([error.description UTF8String]));
			AsyncAction->OnFailure.Broadcast(*resultArray);
		}
	}];
}

void FIOSSavedGameStorage::ShowSelectUI(const FString& Title, bool bAllowCreate, bool bAllowDelete, int32 MaxSnapshots, USavedGameAsyncShowExternalUI* AsyncAction)
{
	UE_LOG(LogCloudSavedGame, Warning, TEXT("ShowSelectUI not implemented for iCloud"));
	AsyncAction->OnFailure.Broadcast();
}

void FIOSSavedGameStorage::ResolveConflict(const FString& SlotName, USaveGame* SaveGame, USavedGameAsyncSaveGame* AsyncAction)
{
	NSString *slotNameIOS = [NSString stringWithUTF8String:TCHAR_TO_UTF8(*SlotName)];
	GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
	[localPlayer fetchSavedGamesWithCompletionHandler:^(NSArray<GKSavedGame *> * _Nullable savedGames, NSError * _Nullable error) {
		if (error == nil) {
			NSMutableArray *foundSaves = [[NSMutableArray alloc]init];
			for (GKSavedGame* savedGame in savedGames) {
				if ([savedGame.name isEqualToString:slotNameIOS]) {
					[foundSaves addObject:savedGame];
				}
			}

			if ([foundSaves count] == 0) {
				UE_LOG(LogCloudSavedGame, Warning, TEXT("Conflict resolving on iCloud failed: Not found any record with name '%s'"), *SlotName);
				AsyncAction->OnFailure.Broadcast();
			}

			TArray<uint8> objectBytes;
			UGameplayStatics::SaveGameToMemory(SaveGame, objectBytes);
			[localPlayer resolveConflictingSavedGames:[foundSaves copy] withData:[NSData dataWithBytes:objectBytes.GetData() length:objectBytes.Num()] completionHandler:^(NSArray<GKSavedGame*>* _Nullable newSavedGames, NSError* _Nullable conflictError) {
				if (conflictError == nil) {
					UE_LOG(LogCloudSavedGame, Log, TEXT("Conflict resolved successful for name '%s'"), *SlotName);
					AsyncAction->OnSuccess.Broadcast();
				}
				else {
					UE_LOG(LogCloudSavedGame, Error, TEXT("Conflict resolving on iCloud failed with code: %i (%s)"), conflictError.code, UTF8_TO_TCHAR([conflictError.description UTF8String]));
					AsyncAction->OnFailure.Broadcast();
				}
			}];
		}
		else {
			UE_LOG(LogCloudSavedGame, Error, TEXT("Fetching on iCloud failed with code: %i (%s)"), error.code, UTF8_TO_TCHAR([error.description UTF8String]));
			AsyncAction->OnFailure.Broadcast();
		}
	}];
}

bool FIOSSavedGameStorage::HasInternetConnection()
{
	// Create zero address
	struct sockaddr_in zeroAddress;
	bzero(&zeroAddress, sizeof(zeroAddress));
	zeroAddress.sin_len = sizeof(zeroAddress);
	zeroAddress.sin_family = AF_INET;

	// Recover reachability flags
	SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
	SCNetworkReachabilityFlags flags;

	BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
	CFRelease(defaultRouteReachability);

	if (!didRetrieveFlags)
		return false;

	BOOL isReachable = flags & kSCNetworkFlagsReachable;
	BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
	return isReachable && !needsConnection;
}