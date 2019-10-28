// Google Play Saved Games
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#include "GooglePlayCloudSaveGame.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

UGooglePlayCloudSaveGame::UGooglePlayCloudSaveGame(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	description_(TEXT("")),
	playedTime_(0),
	progressValue_(0)
{
}

void UGooglePlayCloudSaveGame::SetDescription(const FString & NewDescription)
{
	description_ = NewDescription;
}

void UGooglePlayCloudSaveGame::SetPlayedTime(float NewPlayedTime)
{
	playedTime_ = NewPlayedTime;
}

void UGooglePlayCloudSaveGame::SetProgressValue(int32 NewProgressValue)
{
	progressValue_ = NewProgressValue;
}