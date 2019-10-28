// Google Play Saved Games
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GooglePlayCloudSaveGame.generated.h"

/**
 * Save Game class with Google Play snapshot metadata
 */
UCLASS(Blueprintable, BlueprintType)
class CLOUDSAVEDGAMECORE_API UGooglePlayCloudSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UGooglePlayCloudSaveGame(const class FObjectInitializer& ObjectInitializer);

public:
	/* Set save game description */
	UFUNCTION(BlueprintCallable, Category = "CloudSavedGame|GooglePlay")
	void SetDescription(const FString& NewDescription);

	/* Set overal played time */
	UFUNCTION(BlueprintCallable, Category = "CloudSavedGame|GooglePlay")
	void SetPlayedTime(float NewPlayedTime);

	/* Set player progress value (Not working yet) */
	UFUNCTION(BlueprintCallable, Category = "CloudSavedGame|GooglePlay")
	void SetProgressValue(int32 NewProgressValue);

	/* Get setted description */
	UFUNCTION(BlueprintCallable, Category = "CloudSavedGame|GooglePlay")
	inline FString GetDescription() const { return description_; }

	/* Get setted Played Time in seconds */
	UFUNCTION(BlueprintCallable, Category = "CloudSavedGame|GooglePlay")
	inline float GetPlayedTime() const { return playedTime_; }

	/* Get setted Progress */
	UFUNCTION(BlueprintCallable, Category = "CloudSavedGame|GooglePlay")
	inline int32 GetProgressValue() const { return progressValue_; }

private:
	UPROPERTY(transient)
	FString description_;
	UPROPERTY(transient)
	float playedTime_;
	UPROPERTY(transient)
	int32 progressValue_;
};
