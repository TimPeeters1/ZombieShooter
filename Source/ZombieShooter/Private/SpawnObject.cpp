


#include "SpawnObject.h"

#include "GameFramework/Character.h"


// Sets default values
ASpawnObject::ASpawnObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnObject::BeginPlay()
{
	Super::BeginPlay();
	
}

FTransform ASpawnObject::GetSpawnTransform()
{
	return GetActorTransform();
}

void ASpawnObject::SpawnEnemy(TSubclassOf<class ACharacter> EnemyType, FTransform* SpawnTransform)
{
	//FActorSpawnParameters SpawnParameters;
	//SpawnParameters.Name = MakeUniqueObjectName(this, EnemyType, FName("Zombie_AI"));
	AActor* SpawnedEnemy = (AActor*)GetWorld()->SpawnActor(EnemyType, SpawnTransform);
}

// Called every frame
void ASpawnObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

