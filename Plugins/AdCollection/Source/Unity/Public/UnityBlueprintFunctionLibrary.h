// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnityBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNITY_API UUnityBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	
	/**
	* set current placement
	* @param	placement		
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetCurrentPlacement", Keywords = "Placement"), Category = "Unity")
	static void SetCurrentPlacement(FString Placement);
};
