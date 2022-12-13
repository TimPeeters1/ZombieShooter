// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MetasoundSource.h"
#include "AmbienceBaseLayerData.generated.h"

/** Defines the area around the player that an AmbienceSoundSource can play in. */
USTRUCT(BlueprintType)
struct FAmbienceBaseLayerData
{
	GENERATED_USTRUCT_BODY()

	//Sound
	/** The MetaSound source to use for this baselayer. NOTE: This sound will be played in 2D without any attenuation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = BaseLayerData)
	UMetaSoundSource* Sound;

	//Volume
	/** The volume multiplier to play the base layer with. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = BaseLayerData)
	float Volume;

	//Fade time
	/** The time it takes to fade from the previous baselayer to this base layer. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = BaseLayerData)
	float FadeTime;


	/** Constructor with default values. */
	FAmbienceBaseLayerData()
	{
		Sound = nullptr;
		Volume = 1.0f;
		FadeTime = 1.0f;

	}
};
