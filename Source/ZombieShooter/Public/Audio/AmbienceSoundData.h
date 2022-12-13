// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MetasoundSource.h"
#include "AmbienceSoundData.generated.h"

/** Defines the sounds and behavior for an AmbienceSystem preset entry. */
USTRUCT(BlueprintType)
struct FAmbienceSoundData
{
	GENERATED_USTRUCT_BODY()

	//Sounds
	/** The MetaSoundSource types that can be selected for an AmbienceSystem preset entry. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundData)
	TArray<UMetaSoundSource*> Sounds;

	//Weights
	/** The weights to use when picking a sound from the Sounds array. Leaving this array empty will cause an even distribution of selection for the sounds.
	Similarly, setting this array to a length different from the Sounds array will cause this parameter to be ignored as well. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundData)
	TArray<int> Weights;

	//Object aware placement
	/** If set to true, this sound will try to play at specified object types. Useful to ensure a diegetic relation between a sound and its perceived source.
	NOTE: Sounds of this type are slightly more expensive than non-ObjectAware sounds depending on the number of SourceObject classes,
	as the SoundSource will perform a collision query to find a source object near its location. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundData)
	bool IsObjectAware;

	// Object aware range
	/** The range in which the AmbienceSoundSource should look for a source object if IsObjectAware is set to true. By default, the sound
	will not play if no object was found within the ObjectAwarenessRange.This behavior can be changed by setting PlayIfNoObjectIsFound to true. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundData)
	float ObjectAwarenessRange;

	// Source objects
	/** The objects that the AmbienceSoundSource should search for when IsObjectAware is set to true. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundData)
	TArray<UClass*> SourceObjects;

	// Play if no object is found
	/** If set to true, the AmbienceSoundSource will play at its original specified transform if no source object was found. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundData)
	bool PlayIfNoObjectIsFound;
	



	/** Constructor with default values. */
	FAmbienceSoundData()
	{
		IsObjectAware = false;
		ObjectAwarenessRange = 100;
		PlayIfNoObjectIsFound = false;
	}

};
