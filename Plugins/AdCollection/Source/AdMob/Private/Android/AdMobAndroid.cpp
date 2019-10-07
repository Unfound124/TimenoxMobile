/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#include "AdMob.h"

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Misc/ConfigCacheIni.h"
#include "Async/TaskGraphInterfaces.h"
#include "StringConv.h"

DEFINE_LOG_CATEGORY_STATIC(AdCollection, Log, All);


void FAdMobModule::ShowBanner(enAdsBannerPos pos)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID ShowBannerMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AdMob_ShowBanner", "(Z)V", bIsOptional);
		if (ShowBannerMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_AdMob_ShowBanner not found") );
			return;
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowBannerMethod, pos == enAdsBannerPos::enAdsBannerPos_Bottom);
	}
}

void FAdMobModule::HideBanner()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID HideBannerMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AdMob_HideBanner", "()V", bIsOptional);
		//CHECK_JNI_METHOD(HideBannerMethod);

		if (HideBannerMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_AdMob_HideBanner not found"));
			return;
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, HideBannerMethod);
	}
}


void FAdMobModule::ShowInterstitialAd()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID ShowInterstitialMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AdMob_ShowInterstitialAd", "()V", bIsOptional);

		if (ShowInterstitialMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_AdMob_ShowInterstitialAd not found"));
			return;
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, ShowInterstitialMethod);
	}
}


void FAdMobModule::PlayRewardedVideo()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID PlayRewardVideoMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AdMob_PlayRewardedVideo", "()V", bIsOptional);

		if (PlayRewardVideoMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_AdMob_PlayRewardedVideo not found"));
			return;
		}

		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, PlayRewardVideoMethod);
	}
}

bool FAdMobModule::IsBannerReady()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID JniMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AdMob_BannerReady", "()Z", bIsOptional);
		if (JniMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_AdMob_BannerReady not found"));
			return false;
		}

		// Convert scope array into java fields
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, JniMethod);
	}

	return false;
}

bool FAdMobModule::IsInterstitalReady()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID JniMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AdMob_InterstitialReady", "()Z", bIsOptional);
		if (JniMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_AdMob_InterstitialReady not found"));
			return false;
		}

		// Convert scope array into java fields
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, JniMethod);
	}

	return false;
}

bool FAdMobModule::IsRewardedVideoReady()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID JniMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_AdMob_RewardedVideoReady", "()Z", bIsOptional);
		if (JniMethod == nullptr)
		{
			UE_LOG(AdCollection, Error, TEXT("AndroidThunkJava_AdMob_RewardedVideoReady not found"));
			return false;
		}

		// Convert scope array into java fields
		return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, JniMethod);
	}

	return false;
}

__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_AdMob_nativeInterstitialClick(JNIEnv* jenv, jobject thiz)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.Java_com_ads_util_AdMob_nativeInterstitialClick"), STAT_FSimpleDelegateGraphTask_Java_com_ads_util_AdMob_nativeInterstitialClick, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativeInterstitialClick\n"));
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerInterstitialClickDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_Java_com_ads_util_AdMob_nativeInterstitialClick),
		nullptr,
		ENamedThreads::GameThread
		);
}

__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_AdMob_nativeInterstitialShow(JNIEnv* jenv, jobject thiz)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.Java_com_ads_util_AdMob_nativeInterstitialShow"), STAT_FSimpleDelegateGraphTask_Java_com_ads_util_AdMob_nativeInterstitialShow, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativeInterstitialShow\n"));
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerInterstitialShowDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_Java_com_ads_util_AdMob_nativeInterstitialShow),
		nullptr,
		ENamedThreads::GameThread
		);
}

__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_AdMob_nativeInterstitialClose(JNIEnv* jenv, jobject thiz)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.Java_com_ads_util_AdMob_nativeInterstitialClose"), STAT_FSimpleDelegateGraphTask_Java_com_ads_util_AdMob_nativeInterstitialClose, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativeInterstitialClose\n"));
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerInterstitialClosedDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_Java_com_ads_util_AdMob_nativeInterstitialClose),
		nullptr,
		ENamedThreads::GameThread
		);
}


__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_AdMob_nativePlayRewardedClose(JNIEnv* jenv, jobject thiz)
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.Java_com_ads_util_AdMob_nativePlayRewardedClose"), STAT_FSimpleDelegateGraphTask_Java_com_ads_util_AdMob_nativePlayRewardedClose, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativePlayRewardedClose\n"));
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}

		pModule->TriggerPlayRewardClosedDelegates();
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_Java_com_ads_util_AdMob_nativePlayRewardedClose),
		nullptr,
		ENamedThreads::GameThread
		);
}

__attribute__((visibility("default"))) extern "C" void Java_com_ads_util_AdMob_nativePlayRewardedComplete(JNIEnv* jenv, jobject thiz, jstring type, jint amount)
{
	const char* charsType = jenv->GetStringUTFChars(type, 0);
	FString strHoldType = FString(UTF8_TO_TCHAR(charsType));

	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.nativePlayRewardedComplete"), STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativbe playrewarded completed\n") );
		FAdMobModule* pModule = FModuleManager::Get().LoadModulePtr<FAdMobModule>(TEXT("AdMob"));
		if (pModule == nullptr)
		{
			return;
		}

		FRewardedStatus status;
		status.AdType = EAdType::AdMob;

		
		status.AdMobItem.Type = strHoldType;
		status.AdMobItem.Amount = (int)amount;

		pModule->TriggerPlayRewardCompleteDelegates(status);
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete),
		nullptr,
		ENamedThreads::GameThread
		);
}

void FAdMobModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    
}

void FAdMobModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    
}
