// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AmbiencePlayRangeData.h"
#include "AmbienceSoundData.h"
#include "AmbienceSpatialSoundData.generated.h"

/** Defines the spawning behavior of a spatial AmbienceSoundSource */
USTRUCT(BlueprintType)
struct FAmbienceSpatialSoundData
{
	GENERATED_USTRUCT_BODY()

	//Name
	/** The name of an AmbienceSystem preset entry. Is only used for debugging purposes. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpatialSoundPlayData)
	FString Name;

	//Sounds
	/** Sound data for an AmbienceSystem preset entry. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpatialSoundPlayData)
	FAmbienceSoundData Sounds; 

	//Volume
	/** The volume multiplier for an AmbienceSystem preset entry */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpatialSoundPlayData)
	float Volume;

	//DelayMin
	/** The minimum delay between instances of an AmbienceSystem preset entry occuring. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpatialSoundPlayData)
	float DelayMin;

	//DelayMax
	/** The maximum delay between instances of an AmbienceSystem preset entry occuring. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpatialSoundPlayData)
	float DelayMax;

	//PlayRange
	/** PlayRange data for an AmbienceSystem preset entry. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpatialSoundPlayData)
	FAmbiencePlayRangeData PlayRange;

	//Use
	/** When set to false, this AmbienceSystem preset entry will not play. Useful to temporarily disable preset entries without deleting them. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpatialSoundPlayData)
	bool Use;

	

	/** Constructor with default values. */
	FAmbienceSpatialSoundData()
	{
		Sounds = FAmbienceSoundData();
		Volume = 1.0f;
		DelayMin = 10.0f;
		DelayMax = 30.0f;
		PlayRange = FAmbiencePlayRangeData();
		Use = true;
	}

};
