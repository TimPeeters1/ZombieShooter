// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Player/InteractableObjectInterface.h"
#include "General/GameMode_Main.h"
#include "WeaponObject.h"

#include "WeaponPickupPoint.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AWeaponPickupPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponPickupPoint();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "WeaponPickupPoint", meta = (AllowPrivateAccess = "true"))
		AWeaponObject* CurrentWeaponObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WeaponPickupPoint", meta = (AllowPrivateAccess = "true"))
		TArray<UWeaponData*> SpawnableWeaponTypes;

	/*Minimal Seconds before a new weapon can spawn.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WeaponPickupPoint||Settings", meta = (AllowPrivateAccess = "true"))
		float MinSpawnDelay = 45.f;

	/*Maximal Seconds before a new weapon can spawn.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WeaponPickupPoint||Settings", meta = (AllowPrivateAccess = "true"))
		float MaxSpawnDelay = 120.f;

	/*Maximal Seconds before a new weapon can spawn.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
		float MeshRotationSpeed = 8.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	FTimerHandle SpawnTimerHandle;

public:	
	float GenerateSpawnDelay();

	UFUNCTION()
	void SpawnWeaponPickup();

	UFUNCTION()
	void OnPickup();

	virtual void BeginPlay() override;
	virtual void Tick(float Delta) override;
};
