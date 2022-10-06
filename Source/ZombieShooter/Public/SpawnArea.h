

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SpawnObject.h"

#include "SpawnArea.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ZOMBIESHOOTER_API ASpawnArea : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = "Spawning|SpawnPoints", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ASpawnObject*> SpawnPoints;

	void SpawnEnemies(TSubclassOf<class ACharacter> EnemyToSpawn);
};
