


#include "Spawning/SpawnObject.h"

#include "GameFramework/Character.h"


// Sets default values
ASpawnObject::ASpawnObject()
{
	PrimaryActorTick.bCanEverTick = false;
}

FTransform ASpawnObject::GetSpawnTransform()
{
	return GetActorTransform();
}


