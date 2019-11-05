// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2019 gameDNA Ltd. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MobileUtilsBlueprintLibrary.generated.h"

UCLASS()
class MOBILEUTILS_API UMobileUtilsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/**
	* Check if internet connection is available on a device
	*
	* @return - true if connection is available
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static bool CheckInternetConnection();

	/**
	* Check if Google Play Services are available on a device
	*
	* @return - true if Google Play Services are available
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static bool CheckGooglePlayServices();

	/**
	* Return persistent Unique Device ID without reset after app reinstall
	*
	* @return - Unique Device ID
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static FString GetPersistentUniqueDeviceId();

	/**
	* Return Device ID. Should be unique but not guaranteed.
	*
	* @return - Unique Device ID
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static FString GetDeviceId();

	/**
	* Return platform specific Auth Token retrieved from Online Subsystem
	*
	* @return - Auth Token
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MobileUtils)
	static FString GetAuthToken();
};
