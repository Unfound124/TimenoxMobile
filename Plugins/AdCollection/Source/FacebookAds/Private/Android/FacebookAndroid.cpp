/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#include "Facebook.h"

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Misc/ConfigCacheIni.h"
#include "Async/TaskGraphInterfaces.h"
#include "StringConv.h"

DEFINE_LOG_CATEGORY_STATIC(AdCollection, Log, All);


void FFacebookModule::ShowBanner(enAdsBannerPos pos)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID ShowBannerMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_ShowBanner", "(Z)V", bIsOptional);
		if (ShowBannerMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_Facebook_ShowBanner not found") );
			return;
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowBannerMethod, pos == enAdsBannerPos::enAdsBannerPos_Bottom);
	}
}

void FFacebookModule::HideBanner()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID HideBannerMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_HideBanner", "()V", bIsOptional);
		//CHECK_JNI_METHOD(HideBannerMethod);

		if (HideBannerMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_Facebook_HideBanner not found"));
			return;
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HideBannerMethod);
	}
}


void FFacebookModule::ShowInterstitialAd()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID ShowInterstitialMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_ShowInterstitialAd", "()V", bIsOptional);

		if (ShowInterstitialMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_Facebook_ShowInterstitialAd not found"));
			return;
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowInterstitialMethod);
	}
}


void FFacebookModule::PlayRewardedVideo()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID PlayRewardVideoMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_PlayRewardedVideo", "()V", bIsOptional);

		if (PlayRewardVideoMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_Facebook_PlayRewardedVideo not found"));
			return;
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, PlayRewardVideoMethod);
	}
}

bool FFacebookModule::IsBannerReady()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID JniMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_BannerReady", "()Z", bIsOptional);
		if (JniMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_Facebook_BannerReady not found"));
			return false;
		}

		// Convert scope array into java fields
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, JniMethod);
	}

	return false;
}

bool FFacebookModule::IsInterstitalReady()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID JniMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_InterstitialReady", "()Z", bIsOptional);
		if (JniMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_Facebook_InterstitialReady not found"));
			return false;
		}

		// Convert scope array into java fields
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, JniMethod);
	}

	return false;
}

bool FFacebookModule::IsRewardedVideoReady()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID JniMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Facebook_RewardedVideoReady", "()Z", bIsOptional);
		if (JniMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_Facebook_RewardedVideoReady not found"));
			return false;
		}

		// Convert scope array into java fields
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, JniMethod);
	}

	return false;
}

__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_Facebook_nativeInterstitialClick(JNIEnv* jenv, jobject thiz)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.Java_com_ads_util_Facebook_nativeInterstitialClick"), STAT_FSimpleDelegateGraphTask_Java_com_ads_util_Facebook_nativeInterstitialClick, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativeInterstitialClick\n"));
		FFacebookModule* pModule = FModuleManager::Get().LoadModulePtr<FFacebookModule>(TEXT("FacebookAds"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerInterstitialClickDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_Java_com_ads_util_Facebook_nativeInterstitialClick),
		nullptr,
		ENamedThreads::GameThread
		);
}


__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_Facebook_nativeInterstitialShow(JNIEnv* jenv, jobject thiz)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.Java_com_ads_util_Facebook_nativeInterstitialShow"), STAT_FSimpleDelegateGraphTask_Java_com_ads_util_Facebook_nativeInterstitialShow, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativeInterstitialShow\n"));
		FFacebookModule* pModule = FModuleManager::Get().LoadModulePtr<FFacebookModule>(TEXT("FacebookAds"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerInterstitialShowDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_Java_com_ads_util_Facebook_nativeInterstitialShow),
		nullptr,
		ENamedThreads::GameThread
		);
}


__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_Facebook_nativeInterstitialClose(JNIEnv* jenv, jobject thiz)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.Java_com_ads_util_Facebook_nativeInterstitialClose"), STAT_FSimpleDelegateGraphTask_Java_com_ads_util_Facebook_nativeInterstitialClose, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativeInterstitialClose\n"));
		FFacebookModule* pModule = FModuleManager::Get().LoadModulePtr<FFacebookModule>(TEXT("FacebookAds"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerInterstitialClosedDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_Java_com_ads_util_Facebook_nativeInterstitialClose),
		nullptr,
		ENamedThreads::GameThread
		);
}

__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_Facebook_nativePlayRewardedClose(JNIEnv* jenv, jobject thiz)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.Java_com_ads_util_Facebook_nativePlayRewardedClose"), STAT_FSimpleDelegateGraphTask_Java_com_ads_util_Facebook_nativePlayRewardedClose, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativePlayRewardedClose\n"));
		FFacebookModule* pModule = FModuleManager::Get().LoadModulePtr<FFacebookModule>(TEXT("FacebookAds"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerPlayRewardClosedDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_Java_com_ads_util_Facebook_nativePlayRewardedClose),
		nullptr,
		ENamedThreads::GameThread
		);
}


__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_Facebook_nativePlayRewardedComplete(JNIEnv* jenv, jobject thiz)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.nativePlayRewardedComplete"), STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativbe playrewarded completed\n") );
		FFacebookModule* pModule = FModuleManager::Get().LoadModulePtr<FFacebookModule>(TEXT("FacebookAds"));
		if (pModule == nullptr)
		{
			return;
		}

		FRewardedStatus status;
		status.AdType = EAdType::FacebookAds;


		pModule->TriggerPlayRewardCompleteDelegates(status);
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete),
		nullptr,
		ENamedThreads::GameThread
		);
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
