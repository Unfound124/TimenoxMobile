// Fill out your copyright notice in the Description page of Project Settings.

#include "Unity.h"
#include "UnityBlueprintFunctionLibrary.h"
#import <AdsUtil/AdsUtil.h>


void UUnityBlueprintFunctionLibrary::SetCurrentPlacement(FString placement)
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[[UnityHelper GetDelegate] SetPlacement:[NSString stringWithFString : placement] ];
		});
}

