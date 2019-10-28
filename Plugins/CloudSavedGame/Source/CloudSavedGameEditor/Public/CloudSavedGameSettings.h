// Cloud Saved Game
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CloudSavedGameSettings.generated.h"

/**
 * Plugin Settings class
 */
UCLASS(config = Engine, defaultconfig)
class UCloudSavedGameSettings : public UObject
{
	GENERATED_BODY()

public:
	UCloudSavedGameSettings(const FObjectInitializer& ObjectInitializer);
	// Mark method Resolve Conflict as buffer (dummy method with OnSuccess)
	UPROPERTY(Config, EditAnywhere, Category = "Google Play Libraries", meta = (DisplayName = "Do not actually save game with Resolve Conflict method"))
	bool bAndroidDoNotUseResolveConflict;
	// $(TeamIdentifierPrefix)
	UPROPERTY(Config, EditAnywhere, Category = "IOS", meta = (DisplayName = "iOS Team Identifier"))
	FString TeamIdentifier;
};