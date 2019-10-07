/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#include "Facebook.h"


void FFacebookModule::ShowBanner(enAdsBannerPos pos)
{
}

void FFacebookModule::HideBanner()
{

}

void FFacebookModule::ShowInterstitialAd()
{

}

void FFacebookModule::PlayRewardedVideo()
{
}

bool FFacebookModule::IsBannerReady()
{
	return false;
}

bool FFacebookModule::IsInterstitalReady()
{
	return false;
}

bool FFacebookModule::IsRewardedVideoReady()
{
	return false;
}

void FFacebookModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    
}

void FFacebookModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    
}
