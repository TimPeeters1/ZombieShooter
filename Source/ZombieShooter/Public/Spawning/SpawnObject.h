

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnObject.generated.h"

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API ASpawnObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnObject();
	virtual FTransform GetSpawnTransform();
};
