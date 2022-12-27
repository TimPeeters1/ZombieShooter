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
	// Sets default values for this actor's properties
	AWeaponPickupPoint();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "WeaponPickupPoint", meta = (AllowPrivateAccess = "true"))
		AWeaponObject* CurrentWeaponObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WeaponPickupPoint", meta = (AllowPrivateAccess = "true"))
		TArray<UWeaponData*> SpawnableWeaponTypes;

	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponPickupPoint")
		void SpawnWeaponPickup();
	void SpawnWeaponPickup_Implementation();

	UFUNCTION()
	void OnPickup();

};
