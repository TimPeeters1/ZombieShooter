

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AmbienceBaseLayerData.h"
#include "AmbienceSpatialSoundData.h"
#include "AmbienceSystemPreset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ZOMBIESHOOTER_API UAmbienceSystemPreset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FAmbienceBaseLayerData BaseLayer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FAmbienceSpatialSoundData> SpatialSounds;
};
