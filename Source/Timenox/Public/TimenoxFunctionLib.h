// Timenox Copyright (c) Mutex Studios

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TimenoxFunctionLib.generated.h"

/**
 * 
 */
UCLASS()
class TIMENOX_API UTimenoxFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
		static bool isEmailValid(const FString& email);
};
