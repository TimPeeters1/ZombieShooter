// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"

#include "HealthPickupObject.h"

#include "HealthPickupPoint.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AHealthPickupPoint : public AActor
{
	GENERATED_BODY()

public:
	AHealthPickupPoint();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PickupPoint", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AHealthPickupObject> HealthPickupActor;

protected:
	/*Minimal Seconds before a new weapon can spawn.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PickupPoint||Settings", meta = (AllowPrivateAccess = "true"))
		float MinSpawnDelay = 45.f;

	/*Maximal Seconds before a new weapon can spawn.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PickupPoint||Settings", meta = (AllowPrivateAccess = "true"))
		float MaxSpawnDelay = 120.f;

	/*Maximal Seconds before a new weapon can spawn.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
		float MeshRotationSpeed = 8.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	AHealthPickupObject* CurrentHealthObject;

	float GenerateSpawnDelay();
	FTimerHandle SpawnTimerHandle;
private:

	UFUNCTION()
		void SpawnHealthPickup();

	UFUNCTION()
		void OnPickup();

	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;
};
