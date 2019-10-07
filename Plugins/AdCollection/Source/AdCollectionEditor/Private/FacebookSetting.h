/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#pragma once

#include "UObject/NoExportTypes.h"
#include "FacebookSetting.generated.h"

/**
 * 
 */
UCLASS(transient, config = Engine)
class UFacebookSetting : public UObject
{
	GENERATED_BODY()
	
	// if the android facebook enable
	UPROPERTY(Config, EditAnywhere, Category = Android, Meta = (DisplayName = "If Facebook Android Enable"))
	bool bAndroidEnabled;

	// the banner adunit
	UPROPERTY(Config, EditAnywhere, Category = Android, Meta = (DisplayName = "Android Banner Placement"))
	FString AndroidBannerUnit;

	// the interstitial adunit
	UPROPERTY(Config, EditAnywhere, Category = Android, Meta = (DisplayName = "Android Interstitial Placement"))
	FString AndroidInterstitialUnit;

	// the rewardedvideo adunit
	UPROPERTY(Config, EditAnywhere, Category = Android, Meta = (DisplayName = "Android RewardVideo Placement"))
	FString AndroidRewardedVideoAdUnit;


	UPROPERTY(Config, EditAnywhere, Category = IOS, Meta = (DisplayName = "If Facebook IOS Enable"))
	bool bIOSEnabled;

	UPROPERTY(Config, EditAnywhere, Category = IOS, Meta = (DisplayName = "IOS Banner Placement"))
	FString IOSBannerUnit;

	UPROPERTY(Config, EditAnywhere, Category = IOS, Meta = (DisplayName = "IOS Interstitial Placement"))
	FString IOSInterstitialUnit;

	UPROPERTY(Config, EditAnywhere, Category = IOS, Meta = (DisplayName = "IOS RewardVideo Placement"))
	FString IOSRewardedVideoAdUnit;
    
	
#if WITH_EDITOR
	
	// UObject interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
