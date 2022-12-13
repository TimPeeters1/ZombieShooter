// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AmbiencePlayRangeData.generated.h"


/** Defines the area around the player that an AmbienceSoundSource can play in. */
USTRUCT(BlueprintType)
struct FAmbiencePlayRangeData
{
	GENERATED_USTRUCT_BODY()

	//Offset
	/** Adds an offset to the origin of the AmbienceSystem for a sound type. Useful to ensure that sounds always play in front or above the player. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = PlayRangeData)
	FVector Offset;

	//Horizontal range
	/** Defines the horizontal range in which a sound source can play in relation to the player. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = PlayRangeData)
		float HorizontalRange;

	//Vertical range
	/** Defines the vertical range in which a sound source can play in relation to the player. Is not affected by ExclusionRadius. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = PlayRangeData)
		float VerticalRange;

	//Exclusion radius
	/** Defines a radius around the player in which sound sources cannot play. Sound sources will always play in the range between the HorizontalRadius and ExclusionRadius.
	Useful to ensure that certain sounds are always played a certain distance away from the player. NOTE: This value should not be greater than the value set in HorizontalRange! */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = PlayRangeData)
		float ExclusionRadius;

	//Vertical limits
	/** Defines a vertical range in which sounds are allowed to play. Useful to ensure that certain sounds only play when the player is near the ground or in the sky.
	Leave at 0,0 to ignore this value. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = PlayRangeData)
		FVector2D VerticalLimits;

	/** Constructor with default values. */
	FAmbiencePlayRangeData()
	{
		Offset = FVector();
		HorizontalRange = 1000;
		VerticalRange = 1000;
		ExclusionRadius = 250;
		VerticalLimits = FVector2D();
	}
};