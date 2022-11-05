

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

#include "WeaponData.h"

#include "WeaponObject.generated.h"


UCLASS(Blueprintable, ClassGroup = "Weapon System", meta = (BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API AWeaponObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponObject();

	UPROPERTY(ReplicatedUsing = OnRep_WeaponData, Category = "Weapon Data", EditAnywhere, BlueprintReadWrite)
		UWeaponData* WeaponData;

	UPROPERTY(Category = "Visuals", VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* WeaponModel;

	//Local Ammo Vars will be updated after server callback. To reduce snappy shooting behaviour.
	UPROPERTY(Category = "Ammo|Local", VisibleAnywhere, BlueprintReadOnly)
	uint8 LocalCurrentAmmo;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:
	UPROPERTY(ReplicatedUsing = OnRep_MagazineSizeUpdate, Category = "Ammo", EditAnywhere, BlueprintReadOnly)
		uint8 MagazineSize = 10;
	UPROPERTY(ReplicatedUsing = OnRep_MaxInventoryAmmoUpdate, Category = "Ammo", EditAnywhere, BlueprintReadOnly)
		uint8 MaxInventoryAmmo = 100;

	//Total Ammo in the Weapon's Inventory.
	UPROPERTY(ReplicatedUsing = OnRep_InventoryAmmoUpdate, Category = "Ammo", EditAnywhere, BlueprintReadWrite)
		uint8 InventoryAmmo;

	//Current Ammo in the Weapon's Magazine (Most important!).
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmoUpdate, Category = "Ammo", EditAnywhere, BlueprintReadWrite)
		uint8 CurrentAmmo;


	UFUNCTION()
		void OnRep_WeaponData();

	UFUNCTION()
		void OnRep_MagazineSizeUpdate();
	UFUNCTION()
		void OnRep_MaxInventoryAmmoUpdate();

	UFUNCTION()
		void OnRep_CurrentAmmoUpdate();
	UFUNCTION()
		void OnRep_InventoryAmmoUpdate();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
