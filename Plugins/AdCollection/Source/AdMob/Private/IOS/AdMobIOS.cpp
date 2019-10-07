/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#include "AdMob.h"
#include "IOSAppDelegate.h"
#import <AdsUtil/AdsUtil.h>
#include "Async/TaskGraphInterfaces.h"

void FAdMobModule::ShowBanner(enAdsBannerPos pos)
{
    UIViewController* curViewController = (UIViewController*)[IOSAppDelegate GetDelegate].IOSController;

    dispatch_async(dispatch_get_main_queue(), ^{
        [[AdMobHelper GetDelegate] ShowBanner:curViewController isBottom:pos==enAdsBannerPos::enAdsBannerPos_Bottom];
    });
}

void FAdMobModule::HideBanner()
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [[AdMobHelper GetDelegate] HideBanner];
    });
}

void FAdMobModule::ShowInterstitialAd()
{
    UIViewController* curViewController = (UIViewController*)[IOSAppDelegate GetDelegate].IOSController;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [[AdMobHelper GetDelegate] ShowInterstitialAds:curViewController];
    });
}

void FAdMobModule::PlayRewardedVideo()
{
    UIViewController* curViewController = (UIViewController*)[IOSAppDelegate GetDelegate].IOSController;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [[AdMobHelper GetDelegate] Play:curViewController];
    });
}

bool FAdMobModule::IsBannerReady()
{
    NSMutableDictionary *resultDictionary = [NSMutableDictionary dictionaryWithCapacity:1];
    [[AdMobHelper GetDelegate] performSelectorOnMainThread:@selector(IsBannerReady:) withObject:resultDictionary waitUntilDone:YES];
    
    NSNumber* number = (NSNumber*)[resultDictionary objectForKey:@"RetValue"];
    bool ret = (bool)[number boolValue];
    
    return ret;
}

bool FAdMobModule::IsInterstitalReady()
{
    NSMutableDictionary *resultDictionary = [NSMutableDictionary dictionaryWithCapacity:1];
    [[AdMobHelper GetDelegate] performSelectorOnMainThread:@selector(IsInterstitalReady:) withObject:resultDictionary waitUntilDone:YES];
    
    NSNumber* number = (NSNumber*)[resultDictionary objectForKey:@"RetValue"];
    bool ret = (bool)[number boolValue];
    
    return ret;
}

bool FAdMobModule::IsRewardedVideoReady()
{
    NSMutableDictionary *resultDictionary = [NSMutableDictionary dictionaryWithCapacity:1];
    [[AdMobHelper GetDelegate] performSelectorOnMainThread:@selector(IsPlayable:) withObject:resultDictionary waitUntilDone:YES];
    
    NSNumber* number = (NSNumber*)[resultDictionary objectForKey:@"RetValue"];
    bool ret = (bool)[number boolValue];
    
    return ret;
}

static void IOS_AdMobPlayComplete(NSString* type, int amount)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.nativePlayRewardedComplete"), STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}

		FRewardedStatus status;
		status.AdType = EAdType::AdMob;

		status.AdMobItem.Type = UTF8_TO_TCHAR([type cStringUsingEncoding : NSUTF8StringEncoding]);
		status.AdMobItem.Amount = (int)amount;

		pModule->TriggerPlayRewardCompleteDelegates(status);
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete),
		nullptr,
		ENamedThreads::GameThread
		);
}

static void IOS_AdmobRewardClose()
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.nativeRewardClose"), STAT_FSimpleDelegateGraphTask_nativeRewardClose, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}
		
		pModule->TriggerPlayRewardClosedDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_nativeRewardClose),
		nullptr,
		ENamedThreads::GameThread
		);
}

static void IOS_AdmobInterstitialShow()
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.interstitialShow"), STAT_FSimpleDelegateGraphTask_interstitialShow, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerInterstitialShowDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_interstitialShow),
		nullptr,
		ENamedThreads::GameThread
		);
}
static void IOS_AdmobInterstitialClick()
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.interstitialClick"), STAT_FSimpleDelegateGraphTask_interstitialClick, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerInterstitialClickDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_interstitialClick),
		nullptr,
		ENamedThreads::GameThread
		);
}

static void IOS_AdmobInterstitialClose()
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.interstitialClose"), STAT_FSimpleDelegateGraphTask_interstitialClose, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerInterstitialClosedDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_interstitialClose),
		nullptr,
		ENamedThreads::GameThread
		);
}

void FAdMobModule::StartupModule()
{
    bool isEnable = false;
    
    GConfig->GetBool(TEXT("/Script/AdCollectionEditor.AdMobSetting"), TEXT("bIOSEnabled"), isEnable, GEngineIni);
    if(isEnable)
    {
        FString appId;
        GConfig->GetString(TEXT("/Script/AdCollectionEditor.AdMobSetting"), TEXT("IOSAppId"), appId, GEngineIni);
        
        FString bannerUnit;
        GConfig->GetString(TEXT("/Script/AdCollectionEditor.AdMobSetting"), TEXT("IOSBannerUnit"), bannerUnit, GEngineIni);
        
        FString InterstitalUnit;
        GConfig->GetString(TEXT("/Script/AdCollectionEditor.AdMobSetting"), TEXT("IOSInterstitialUnit"), InterstitalUnit, GEngineIni);
        
        FString rewardedUnit;
        GConfig->GetString(TEXT("/Script/AdCollectionEditor.AdMobSetting"), TEXT("IOSRewardedVideoAdUnit"), rewardedUnit, GEngineIni);
        
        
        UIViewController* curViewController = (UIViewController*)[IOSAppDelegate GetDelegate].IOSController;
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [[AdMobHelper GetDelegate] InitSDK:curViewController AppID:[NSString stringWithFString:appId] BannerUnit:[NSString stringWithFString:bannerUnit]  
			InterstitalUnit:[NSString stringWithFString:InterstitalUnit] RewardedUnit:[NSString stringWithFString:rewardedUnit] 
			callback:&IOS_AdMobPlayComplete rewardClose:&IOS_AdmobRewardClose  interstitialShow:&IOS_AdmobInterstitialShow 
			interstitialClick:&IOS_AdmobInterstitialClick interstitialClose:IOS_AdmobInterstitialClose];
        });
    }
}

void FAdMobModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    
}
