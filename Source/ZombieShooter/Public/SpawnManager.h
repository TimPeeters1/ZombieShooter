

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnArea.h"

#include "SpawnManager.generated.h"

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	UPROPERTY(Category = "Spawning|Enemies", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class ACharacter>> SpawnableEnemies;

	UPROPERTY(Category = "Spawning|Areas", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ASpawnArea*> SpawnAreas;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	void SpawnEnemies();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
