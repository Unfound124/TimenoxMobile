// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavedGameMetadata.generated.h"

/**
 * Metadata for USaveGame to represent Cloud Saved Data
 */
USTRUCT(BlueprintType)
struct CLOUDSAVEDGAMECORE_API FSavedGameMetadata
{
	GENERATED_BODY()

	// Save Data name (or FileName)
	UPROPERTY(BlueprintReadOnly, Category = "CloudSavedGame")
	FString SlotName;
	// Last Modification date of Saved Data
	UPROPERTY(BlueprintReadOnly, Category = "CloudSavedGame")
	FDateTime LastUpdateTime;
	// iOS speciefic value, true if this save from current device
	UPROPERTY(BlueprintReadOnly, Category = "CloudSavedGame|IOS")
	uint32 bIsOwningDevice:1;
	// Android speciefic value
	UPROPERTY(BlueprintReadOnly, Category = "CloudSavedGame|GooglePlay")
	float PlayedTime;
	// Android speciefic value
	UPROPERTY(BlueprintReadOnly, Category = "CloudSavedGame|GooglePlay")
	int32 ProgressValue;
	// Android speciefic value
	UPROPERTY(BlueprintReadOnly, Category = "CloudSavedGame|GooglePlay")
	FString Description;

	FSavedGameMetadata(const FString& SlotNameI = TEXT(""), FDateTime UpdateTime = FDateTime(), bool bOwningDevice = false, float PlayedTimeI = 0.f, int32 ProgressValueI = 0, const FString& DescriptionI = TEXT("")) :
		SlotName(SlotNameI),
		LastUpdateTime(UpdateTime),
		bIsOwningDevice(bOwningDevice),
		PlayedTime(PlayedTimeI),
		ProgressValue(ProgressValueI),
		Description(DescriptionI)
	{
	}
};