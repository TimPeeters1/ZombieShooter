

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "GameMode_Main.h"
#include "PlayerPawn.h"
#include "SpawnArea.h"
#include "SpawnObject.h"

#include "SpawnManager.generated.h"

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API ASpawnManager : public AActor
{
	GENERATED_BODY()

public:
	ASpawnManager();

	//Wave Settings
	UPROPERTY(Category = "Spawning|Enemies|WaveSettings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		uint8 MinWaveSize = 20;
	UPROPERTY(Category = "Spawning|Enemies|WaveSettings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		uint8 MaxWaveSize = 30;

	UPROPERTY(Category = "Spawning|Enemies", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<class ACharacter>> SpawnableEnemies;

	UPROPERTY(Category = "Spawning|Active Areas", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<ASpawnArea*> SpawnAreas;

	UPROPERTY(Category = "Spawning|Active Areas", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float AreaSweepRange = 1500.0f;
	UPROPERTY(Category = "Spawning|Active Areas", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float AreaSweepTimer = 2.0f;
	UPROPERTY(Category = "Spawning|Active Areas", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bDrawDebug = false;

	UPROPERTY(Category = "Spawning|Active Areas", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ASpawnArea*> ActiveAreaSet;

protected:
	FTimerHandle PlayerSweepTimer;


	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "InitWave"), Category = "Spawning")
	void InitWave();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AreaSweep"), Category = "Spawning")
	void AreaSweep();

	virtual void Tick(float DeltaTime) override;

};
