


#include "SpawnObject.h"

#include "GameFramework/Character.h"


// Sets default values
ASpawnObject::ASpawnObject()
{
	PrimaryActorTick.bCanEverTick = false;
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

void ASpawnObject::SpawnEnemy(TSubclassOf<class ACharacter> EnemyType)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Name = MakeUniqueObjectName(this, EnemyType, FName("Zombie_AI"));
	GetWorld()->SpawnActor<ACharacter>(EnemyType, this->GetActorLocation(), FRotator::ZeroRotator);
}

// Called every frame
void ASpawnObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

