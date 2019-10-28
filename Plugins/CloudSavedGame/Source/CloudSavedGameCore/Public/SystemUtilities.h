// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include <vector>

/**
 * Utilities to operate with USaveGame and conversions to native types
 */
class CLOUDSAVEDGAMECORE_API USystemUtilities
{
public:
	/* Cast Raw Data to USaveGame class */
	static USaveGame* SaveGameFromMemory(const TArray<uint8> &ReadData);
	/* Convert TArray to std::vector */
	static bool TArrayToVector(const TArray<uint8>& InData, std::vector<uint8>& OutData);
	/* Convert std::vector to TArray */
	static bool VectorToTArray(const std::vector<uint8>& InData, TArray<uint8>& OutData);
};