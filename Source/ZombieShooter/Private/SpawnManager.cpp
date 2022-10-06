


#include "SpawnManager.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	InitWave();
}

void ASpawnManager::InitWave()
{
	if (SpawnAreas.Num() > 0) {
		for (uint8 i = 0; i < SpawnAreas.Num(); i++)
		{
			SpawnAreas[i]->SpawnEnemies(SpawnableEnemies[0]);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("SPAWNMANAGER: No SpawnAreas Found!"));
	}
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

