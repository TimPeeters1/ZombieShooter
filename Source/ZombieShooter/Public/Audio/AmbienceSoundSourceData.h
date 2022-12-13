// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MetasoundSource.h"
#include "AmbienceSoundSourceData.generated.h"


/** Contains data that can be used by an AmbienceSoundSource instance. */
USTRUCT(BlueprintType)
struct FAmbienceSoundSourceData
{
	GENERATED_USTRUCT_BODY()

	// Sound
	/** The MetaSoundSource to use for an AmbienceSoundSource. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundSourceData)
	UMetaSoundSource* Sound;

	// Volume
	/** The volume to play an AmbienceSoundSource at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundSourceData)
	float Volume;

	// Transform
	/** The transform to play an AmbienceSoundSource at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundSourceData)
	FTransform Transform;

	// Is object aware
	/** When set to true, the AmbienceSoundSource will try to locate an object of any of the specified classes, and play at this location.
	 This is useful for sounds that require a clear source for the player.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundSourceData)
	bool IsObjectAware;

	// Object awareness range
	/** The range in which the AmbienceSoundSource should look for a source object if IsObjectAware is set to true. By default, the sound
	 will not play if no object was found within the ObjectAwarenessRange. This behavior can be changed by setting PlayIfNoObjectIsFound to true.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundSourceData)
	float ObjectAwarenessRange;

	// Source objects
	/** The objects that the AmbienceSoundSource should search for when IsObjectAware is set to true. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundSourceData)
	TArray<UClass*> SourceObjects;

	// Play if no object is found
	/** If set to true, the AmbienceSoundSource will play at its original specified transform if no source object was found. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SoundSourceData)
	bool PlayIfNoObjectIsFound;



	/** Constructor with default values. */
	FAmbienceSoundSourceData()
	{
		Sound = nullptr ;
		Volume = 1.0f;
		Transform = FTransform();
		IsObjectAware = false;
		ObjectAwarenessRange = 1000;
		PlayIfNoObjectIsFound = false;
	}

};
