

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
private:
	UPROPERTY(Category = "Spawning|AreaSettings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 DesiredEnemyCount = 5;

	UPROPERTY(Category = "Spawning|ActiveArea", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bAreaActive = false;

public:
	UPROPERTY(Category = "Spawning", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ASpawnObject*> SpawnPoints;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnEnemies"), Category = "Spawning")
	void SpawnEnemies(TSubclassOf<class ACharacter> EnemyToSpawn);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAreaStatus"), Category = "Spawning|ActiveArea")
	bool GetAreaStatus();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetAreaStatus"), Category = "Spawning|ActiveArea")
	bool SetAreaStatus(bool bActive);

	virtual void BeginPlay() override;
};
