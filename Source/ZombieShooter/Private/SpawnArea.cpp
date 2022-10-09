#include "SpawnArea.h"

void ASpawnArea::BeginPlay()
{
	Super::BeginPlay();

	//TODO Set true via Players! SpawnArea.cpp
	//SetAreaStatus(true);
}

void ASpawnArea::SpawnEnemies(TSubclassOf<class ACharacter> EnemyToSpawn)
{
	if (SpawnPoints.Num() > 0) {
		for (uint8 i = 0; i < SpawnPoints.Num(); i++)
		{
			FTransform SpawnTransform = SpawnPoints[i]->GetSpawnTransform();
			SpawnPoints[i]->SpawnEnemy(EnemyToSpawn, &SpawnTransform);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("SPAWNAREA %s: No SpawnObjects Found!"), *this->GetName());
	}
}

bool ASpawnArea::GetAreaStatus()
{
	return bAreaActive;
}

bool ASpawnArea::SetAreaStatus(bool bActive)
{
	bAreaActive = bActive;
	return bAreaActive;
}

bool ASpawnArea::ContainsPlayers()
{
	return bContainsPlayers;
}

bool ASpawnArea::SetContainsPlayers(bool bActive)
{
	bContainsPlayers = bActive;
	return bContainsPlayers;
}

