

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

#include "General/GameInstance_Main.h"

#include "Characters/Player/PlayerPawn.h"
#include "Spawning/SpawnArea.h"
#include "Spawning/SpawnObject.h"


#include "SpawnManager.generated.h"

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API ASpawnManager : public AActor
{
	GENERATED_BODY()

public:
	/*
	* Max Amount of AI Units that may be present in game at all times. (PERFORMANCE!)
	*/
	UPROPERTY(Category = "Spawning", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		uint8 Max_AI_Population = 50;

	//Replace with Array in the future?
	UPROPERTY(Category = "Spawning", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		uint8 Current_AI_Population;

	UPROPERTY(Category = "Spawning|Active Areas", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float PopulationCheckInterval = 3.0f;

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
		float ActiveAreaSweepInterval = 2.0f;
	UPROPERTY(Category = "Spawning|Active Areas", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bDrawDebug = false;

	//AreaSweep Vars
	UPROPERTY(Category = "Spawning|Active Areas", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<ASpawnArea*> ActiveAreaSet;

	UPROPERTY(Category = "Spawning|Active Areas", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<ASpawnArea*> ActivePlayerArea;

protected:
	UGameInstance_Main* GameInstance;

	FTimerHandle ActiveAreaSweepTimer;
	FTimerHandle PopCheckTimer;

	virtual void BeginPlay() override;

public:
	void StartSpawningRoutines(float StartDelay);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CheckPopulation"), Category = "Spawning")
		void CheckPopulation();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ActiveAreaSweep"), Category = "Spawning")
		void ActiveAreaSweep();

	void DecreaseAI_Population();

	UFUNCTION(BlueprintCallable, meta = (Displayname = "ToggleZombieSpawning"), Category = "Spawning")
	void ToggleZombieSpawning();

};
