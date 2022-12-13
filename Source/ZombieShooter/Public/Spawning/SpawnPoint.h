

#pragma once

#include "CoreMinimal.h"
#include "SpawnObject.h"

#include "Components/ArrowComponent.h"

#include "SpawnPoint.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class ZOMBIESHOOTER_API ASpawnPoint : public ASpawnObject
{
	GENERATED_BODY()
private:
	ASpawnPoint();

	UArrowComponent* ArrowIndicator;

public:
	virtual FTransform GetSpawnTransform() override;
};
