/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#include "ChartBoost.h"
#include "IOSAppDelegate.h"
#import <AdsUtil/AdsUtil.h>
#include "Async/TaskGraphInterfaces.h"

void FChartBoostModule::PlayRewardedVideo()
{
    UIViewController* curViewController = (UIViewController*)[IOSAppDelegate GetDelegate].IOSController;
    [[ChartBoostHelper GetDelegate] performSelectorOnMainThread:@selector(Play:) withObject:curViewController waitUntilDone:NO];
}

void FChartBoostModule::ShowInterstitialAd()
{
    [[ChartBoostHelper GetDelegate] performSelectorOnMainThread:@selector(ShowInterstitialAds) withObject:nil waitUntilDone:NO];
}

bool FChartBoostModule::IsInterstitalReady()
{
    NSMutableDictionary *resultDictionary = [NSMutableDictionary dictionaryWithCapacity:1];
    [[ChartBoostHelper GetDelegate] performSelectorOnMainThread:@selector(IsInterstitalReady:) withObject:resultDictionary waitUntilDone:YES];
    
    NSNumber* number = (NSNumber*)[resultDictionary objectForKey:@"RetValue"];
    bool ret = (bool)[number boolValue];
    
    return ret;
}

bool FChartBoostModule::IsRewardedVideoReady()
{
    NSMutableDictionary *resultDictionary = [NSMutableDictionary dictionaryWithCapacity:1];
    [[ChartBoostHelper GetDelegate] performSelectorOnMainThread:@selector(IsPlayable:) withObject:resultDictionary waitUntilDone:YES];
    
    NSNumber* number = (NSNumber*)[resultDictionary objectForKey:@"RetValue"];
    bool ret = (bool)[number boolValue];
    
    return ret;
}


static void IOS_ChartBoostPlayComplete(int amount)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.nativePlayRewardedComplete"), STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FChartBoostModule* pModule = FModuleManager::Get().LoadModulePtr<FChartBoostModule>(TEXT("ChartBoost"));
		if (pModule == nullptr)
		{
			return;
		}

		FRewardedStatus status;
		status.AdType = EAdType::ChartBoost;
		status.ChartBoostReward = (int)amount;

		pModule->TriggerPlayRewardCompleteDelegates(status);
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete),
		nullptr,
		ENamedThreads::GameThread
		);
}


static void IOS_ChartboostRewardClose()
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.ChartboostRewardClose"), STAT_FSimpleDelegateGraphTask_ChartboostRewardClose, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FChartBoostModule* pModule = FModuleManager::Get().LoadModulePtr<FChartBoostModule>(TEXT("ChartBoost"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerPlayRewardClosedDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_ChartboostRewardClose),
		nullptr,
		ENamedThreads::GameThread
		);
}

static void IOS_ChartboostInterstitialShow()
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.interstitialShow"), STAT_FSimpleDelegateGraphTask_interstitialShow, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FChartBoostModule* pModule = FModuleManager::Get().LoadModulePtr<FChartBoostModule>(TEXT("ChartBoost"));
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
static void IOS_ChartboostInterstitialClick()
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.interstitialClick"), STAT_FSimpleDelegateGraphTask_interstitialClick, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FChartBoostModule* pModule = FModuleManager::Get().LoadModulePtr<FChartBoostModule>(TEXT("ChartBoost"));
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

static void IOS_ChartboostInterstitialClose()
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.interstitialClose"), STAT_FSimpleDelegateGraphTask_interstitialClose, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FChartBoostModule* pModule = FModuleManager::Get().LoadModulePtr<FChartBoostModule>(TEXT("ChartBoost"));
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


void FChartBoostModule::StartupModule()
{
    bool isEnable = false;
    
    GConfig->GetBool(TEXT("/Script/AdCollectionEditor.ChartBoostSetting"), TEXT("bIsIOSEnabled"), isEnable, GEngineIni);
    if(isEnable)
    {
        FString appId;
        GConfig->GetString(TEXT("/Script/AdCollectionEditor.ChartBoostSetting"), TEXT("IOSAppId"), appId, GEngineIni);
        
        FString appSignature;
        GConfig->GetString(TEXT("/Script/AdCollectionEditor.ChartBoostSetting"), TEXT("IOSSignature"), appSignature, GEngineIni);
        
        dispatch_async(dispatch_get_main_queue(), ^{
            [[ChartBoostHelper GetDelegate] InitSDK:[NSString stringWithFString:appId] AppSignature:[NSString stringWithFString:appSignature] callback:&IOS_ChartBoostPlayComplete 
			rewardClose : &IOS_ChartboostRewardClose  interstitialShow : &IOS_ChartboostInterstitialShow
			interstitialClick : &IOS_ChartboostInterstitialClick interstitialClose : IOS_ChartboostInterstitialClose];
        });
    }
}

void FChartBoostModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    
}
