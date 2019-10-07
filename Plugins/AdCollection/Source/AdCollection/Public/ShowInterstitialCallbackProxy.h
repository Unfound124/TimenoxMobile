// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AdCollectionBPLibrary.h"
#include "AdCollection.h"
#include "ShowInterstitialCallbackProxy.generated.h"

/**
 * 
 */
UCLASS()
class ADCOLLECTION_API UShowInterstitialCallbackProxy : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	UShowInterstitialCallbackProxy();

	UPROPERTY(BlueprintAssignable)
	FDynamicInterstitialShowDelegate OnShow;

	UPROPERTY(BlueprintAssignable)
	FDynamicInterstitialClickDelegate OnClick;

	UPROPERTY(BlueprintAssignable)
	FDynamicInterstitialCloseDelegate OnClose;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShowInterstitial", BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "AdCollection")
	static UShowInterstitialCallbackProxy* ShowInterstitial(EAdType AdType);

	virtual void Activate() override;
	static IAdModuleInterface* FindAdsModule(EAdType adType);

	EAdType AdType;


private:

	FInterstitialShowDelegate ShowDelegate;
	FDelegateHandle ShowDelegateHandle;

	FInterstitialClickDelegate ClickDelegate;
	FDelegateHandle ClickDelegateHandle;

	FInterstitialClosedDelegate CloseDelegate;
	FDelegateHandle CloseDelegateHandle;

	void OnShowCallback();
	void OnClickCallback();
	void OnCloseCallback();
};
