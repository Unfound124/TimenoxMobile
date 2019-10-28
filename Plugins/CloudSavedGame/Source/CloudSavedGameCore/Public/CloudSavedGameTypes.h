// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CloudSavedGameTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCloudSavedGame, Log, All);

/**
 * Cloud operations policies
 */
UENUM(BlueprintType)
enum class ECloudSavedGamePolicy : uint8
{
	CSGP_MOST_RECENTLY_MODIFIED UMETA(DisplayName = "Most Recently Modified"),
	CSGP_MANUAL UMETA(DisplayName = "Manual"),
	CSGP_HIGHEST_PROGRESS UMETA(DisplayName = "Highest Progress"),
	CSGP_LONGEST_PLAYTIME UMETA(DisplayName = "Longest Playtime"),
	CSGP_OWNING_DEVICE UMETA(DisplayName = "Owning Device")
};