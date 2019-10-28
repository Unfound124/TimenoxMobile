// Cloud Storage for Mobile
// Created by Vladimir Ziablitskii
// Copyright 2018-2019 Sibirianit All Rights Reserved.

#include "SystemUtilities.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/CustomVersion.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Misc/EngineVersion.h"
#include "UObject/Package.h"

// Use copypaste from GameplayStatics.cpp
static const int UE4_SAVEGAME_FILE_TYPE_TAG = 0x53415647;		// "sAvG"
struct FSaveGameFileVersion
{
	enum Type
	{
		InitialVersion = 1,
		// serializing custom versions into the savegame data to handle that type of versioning
		AddedCustomVersions = 2,

		// -----<new versions can be added above this line>-------------------------------------------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};
};
// End copypaste from GameplayStatics

USaveGame* USystemUtilities::SaveGameFromMemory(const TArray<uint8>& ReadData)
{
	USaveGame* OutSaveGameObject = NULL;

	if (ReadData.Num() > 0)
	{
		FMemoryReader MemoryReader(ReadData, true);

		int32 FileTypeTag;
		MemoryReader << FileTypeTag;

		int32 SavegameFileVersion;
		if (FileTypeTag != UE4_SAVEGAME_FILE_TYPE_TAG) // TODO
		{
			// this is an old saved game, back up the file pointer to the beginning and assume version 1
			MemoryReader.Seek(0);
			SavegameFileVersion = FSaveGameFileVersion::InitialVersion; // TODO

																		// Note for 4.8 and beyond: if you get a crash loading a pre-4.8 version of your savegame file and 
																		// you don't want to delete it, try uncommenting these lines and changing them to use the version 
																		// information from your previous build. Then load and resave your savegame file.
																		//MemoryReader.SetUE4Ver(MyPreviousUE4Version);				// @see GPackageFileUE4Version
																		//MemoryReader.SetEngineVer(MyPreviousEngineVersion);		// @see FEngineVersion::Current()
		}
		else
		{
			// Read version for this file format
			MemoryReader << SavegameFileVersion;

			// Read engine and UE4 version information
			int32 SavedUE4Version;
			MemoryReader << SavedUE4Version;

			FEngineVersion SavedEngineVersion;
			MemoryReader << SavedEngineVersion;

			MemoryReader.SetUE4Ver(SavedUE4Version);
			MemoryReader.SetEngineVer(SavedEngineVersion);

			if (SavegameFileVersion >= FSaveGameFileVersion::AddedCustomVersions) // TODO
			{
				int32 CustomVersionFormat;
				MemoryReader << CustomVersionFormat;

				FCustomVersionContainer CustomVersions;
				CustomVersions.Serialize(MemoryReader, static_cast<ECustomVersionSerializationFormat::Type>(CustomVersionFormat));
				MemoryReader.SetCustomVersions(CustomVersions);
			}
		}

		// Get the class name
		FString SaveGameClassName;
		MemoryReader << SaveGameClassName;

		// Try and find it, and failing that, load it
		UClass* SaveGameClass = FindObject<UClass>(ANY_PACKAGE, *SaveGameClassName);
		if (SaveGameClass == NULL)
		{
			SaveGameClass = LoadObject<UClass>(NULL, *SaveGameClassName);
		}

		// If we have a class, try and load it.
		if (SaveGameClass != NULL)
		{
			OutSaveGameObject = NewObject<USaveGame>(GetTransientPackage(), SaveGameClass);

			FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
			OutSaveGameObject->Serialize(Ar);
		}
	}

	return OutSaveGameObject;
}

bool USystemUtilities::TArrayToVector(const TArray<uint8>& InData, std::vector<uint8>& OutData)
{
	OutData.resize(InData.Num());
	for (int i = 0; i < InData.Num(); i++) {
		OutData[i] = InData[i];
	}

	return OutData.size() > 0;
}

bool USystemUtilities::VectorToTArray(const std::vector<uint8>& InData, TArray<uint8>& OutData)
{
	OutData.SetNumUninitialized(InData.size());
	for (unsigned int i = 0; i < InData.size(); i++) {
		OutData[i] = InData[i];
	}

	return OutData.Num() > 0;
}
