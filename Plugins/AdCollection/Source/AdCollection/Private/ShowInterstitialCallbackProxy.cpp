// Fill out your copyright notice in the Description page of Project Settings.

#include "AdCollection.h"
#include "ShowInterstitialCallbackProxy.h"


UShowInterstitialCallbackProxy::UShowInterstitialCallbackProxy():
	ShowDelegate(FInterstitialShowDelegate::CreateUObject(this, &ThisClass::OnShowCallback)),
	ClickDelegate(FInterstitialClickDelegate::CreateUObject(this, &ThisClass::OnClickCallback)),
	CloseDelegate(FInterstitialClosedDelegate::CreateUObject(this, &ThisClass::OnCloseCallback))
{

}

UShowInterstitialCallbackProxy* UShowInterstitialCallbackProxy::ShowInterstitial(EAdType AdType)
{
	IAdModuleInterface* Module = FindAdsModule(AdType);
	if (Module != NULL)
	{
		UShowInterstitialCallbackProxy* Proxy = NewObject<UShowInterstitialCallbackProxy>();
		Proxy->AdType = AdType;

		return Proxy;
	}

	return nullptr;
}


void UShowInterstitialCallbackProxy::Activate()
{
	IAdModuleInterface* Module = FindAdsModule(AdType);
	if (Module != NULL)
	{
		Module->ClearAllInterstitialShowDelegate_Handle();
		Module->ClearAllInterstitialClickDelegate_Handle();
		Module->ClearAllInterstitialClosedDelegate_Handle();


		ShowDelegateHandle = Module->AddInterstitialShowDelegate_Handle(ShowDelegate);
		ClickDelegateHandle = Module->AddInterstitialClickDelegate_Handle(ClickDelegate);
		CloseDelegateHandle = Module->AddInterstitialClosedDelegate_Handle(CloseDelegate);
		
		Module->ShowInterstitialAd();
	}
}

IAdModuleInterface* UShowInterstitialCallbackProxy::FindAdsModule(EAdType adType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAdType"), true);
	if (!EnumPtr)
	{
		return nullptr;
	}

	FString EnumName = EnumPtr->GetNameByValue((int64)adType).ToString();

	FName adPlatformName;
	int32 ScopeIndex = EnumName.Find(TEXT("::"), ESearchCase::CaseSensitive);
	if (ScopeIndex != INDEX_NONE)
	{
		adPlatformName = FName(*EnumName.Mid(ScopeIndex + 2));
	}
	else
	{
		adPlatformName = FName(*EnumName);
	}

	IAdModuleInterface * Module = FModuleManager::Get().LoadModulePtr<IAdModuleInterface>(adPlatformName);

	return Module;
}

void UShowInterstitialCallbackProxy::OnShowCallback()
{
	IAdModuleInterface* Module = FindAdsModule(AdType);
	if (Module == nullptr)
	{
		return;
	}

	OnShow.Broadcast();

	Module->ClearInterstitialShowDelegate_Handle(ShowDelegateHandle);
}

void UShowInterstitialCallbackProxy::OnClickCallback()
{
	IAdModuleInterface* Module = FindAdsModule(AdType);
	if (Module == nullptr)
	{
		return;
	}

	OnClick.Broadcast();

	Module->ClearInterstitialClickDelegate_Handle(ClickDelegateHandle);
}

void UShowInterstitialCallbackProxy::OnCloseCallback()
{
	IAdModuleInterface* Module = FindAdsModule(AdType);
	if (Module == nullptr)
	{
		return;
	}

	OnClose.Broadcast();

	Module->ClearInterstitialClosedDelegate_Handle(CloseDelegateHandle);
}
