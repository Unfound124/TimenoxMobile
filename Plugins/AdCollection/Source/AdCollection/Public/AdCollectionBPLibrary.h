/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#pragma once

#include "Engine.h"
#include "AdCollectionBPLibrary.generated.h"


UENUM(BlueprintType)		
enum class EAdType : uint8
{
	AdMob 	UMETA(DisplayName = "AdMob"),	// the admob ads type
	Unity 	UMETA(DisplayName = "Unity"),	// the unity ads type
	ChartBoost 	UMETA(DisplayName = "ChartBoost"),	// the chartboost ads type
	FacebookAds  UMETA(DisplayName = "Facebook")
};


USTRUCT(BlueprintType)
struct FAdMobRewardItem
{
	GENERATED_USTRUCT_BODY()
	
	/** the admob rewarded item type*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Type;

	/** the admob rewarded item value*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Amount;
};


USTRUCT(BlueprintType)
struct FRewardedStatus
{
	GENERATED_USTRUCT_BODY()
	
	/** the rewarded ads type*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAdType AdType;

	/** if the AdType is AdMob, the AdmobItem is valid*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FAdMobRewardItem  AdMobItem;

	/** if the AdType is ChartBoost the value is the reward value*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ChartBoostReward;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayRewardedDelegate, FRewardedStatus, RewardStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicRewardedClosedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicInterstitialShowDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicInterstitialClickDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicInterstitialCloseDelegate);

UCLASS()
class UAdCollectionBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:


	/**
	* play rewardedvideo ads
	* @param	adType			the ads type
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SimplePlayRewardedVideo", Keywords = "AdCollection Play"), Category = "AdCollection")
	static void PlayAdVideo(EAdType adType);

	/**
	* Show  banner
	* @param	isOnBottom		if the banner show on the bottom of the screen
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShowBanner", Keywords = "AdCollection Show Banner"), Category = "AdCollection")
	static void ShowBanner(EAdType adType, bool isOnBottom);

	/**
	* hide  banner
	* @param	adType			the  ads type
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HideBanner", Keywords = "AdCollection Hide Banner"), Category = "AdCollection")
	static void HideBanner(EAdType adType);

	/**
	* show interstitial ads
	* @param	adType			the  ads type
	*/
	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShowInterstitial", Keywords = "AdCollection Show Interstitial Ads"), Category = "AdCollection")
	//static void ShowInterstitial(EAdType adType);

	/**
	* check is the banner is load finish
	* @param	adType			the  ads type
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsBannerAdsReady", Keywords = "AdCollection Check Banner Ads Ready"), Category = "AdCollection")
	static bool IsBannerReady(EAdType adType);

	/**
	* check is the interstitial is load finish
	* @param	adType			the  ads type
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsInterstitialAdsReady", Keywords = "AdCollection Check Interstital Ads Ready"), Category = "AdCollection")
	static bool IsInterstitialReady(EAdType adType);

	/**
	* check is the rewardedvideo ads load finish
	* @param	adType			the  ads type
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsRewardedVideoAdsReady", Keywords = "AdCollection Check RewardedVideo Ads Ready"), Category = "AdCollection")
	static bool IsRewardedVideoReady(EAdType adType);
};
