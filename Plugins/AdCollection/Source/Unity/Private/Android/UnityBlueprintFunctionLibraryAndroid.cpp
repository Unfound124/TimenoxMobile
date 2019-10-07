// Fill out your copyright notice in the Description page of Project Settings.

#include "Unity.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "UnityBlueprintFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(Unity, Log, All);

void UUnityBlueprintFunctionLibrary::SetCurrentPlacement(FString Placement)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		const bool bIsOptional = false;
		static jmethodID SetPlacementMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_Unity_setCurrentPlacement", "(Ljava/lang/String;)V", bIsOptional);

		if (SetPlacementMethod == nullptr)
		{
			UE_LOG(Unity, Error, TEXT("AndroidThunkJava_Unity_setCurrentPlacement not found"));
			return;
		}

		jstring PlacementJava = Env->NewStringUTF(TCHAR_TO_UTF8(*Placement));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, SetPlacementMethod, PlacementJava);
		Env->DeleteLocalRef(PlacementJava);
	}
}

