

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnObject.generated.h"

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API ASpawnObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnObject();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void SpawnEnemy(TSubclassOf<class ACharacter> EnemyType);

	virtual FTransform GetSpawnTransform();

	virtual void Tick(float DeltaTime) override;

};
