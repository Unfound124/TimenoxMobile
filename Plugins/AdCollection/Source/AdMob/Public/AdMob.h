/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#pragma once

#include "Modules/ModuleManager.h"
#include "AdCollection.h"

class FAdMobModule : public IAdModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void ShowBanner(enAdsBannerPos pos) override;
	virtual void HideBanner() override;
	virtual void PlayRewardedVideo() override;
	virtual void ShowInterstitialAd() override;

	virtual bool IsBannerReady() override;
	virtual bool IsInterstitalReady() override;
	virtual bool IsRewardedVideoReady() override;
};