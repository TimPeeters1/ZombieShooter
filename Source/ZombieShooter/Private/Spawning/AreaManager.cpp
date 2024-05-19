


#include "Spawning/AreaManager.h"

// Sets default values
AAreaManager::AAreaManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAreaManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAreaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

