#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

#include "Interaction/GenericInteractionActor.h"
#include "WeaponData.h"

#include "WeaponObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponSpawned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponPickup);

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AWeaponObject : public AGenericInteractionActor
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

	virtual void OnStartInteract_BP_Implementation(AActor* InstigatingActor) override;

	virtual void StartHover_BP_Implementation(AActor* InstigatingActor) override;

	virtual void StopHover_BP_Implementation(AActor* InstigatingActor) override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:
	UPROPERTY(ReplicatedUsing = OnRep_MagazineSizeUpdate, Category = "Ammo", EditAnywhere, BlueprintReadOnly)
		int32 MagazineSize = 10;
	UPROPERTY(ReplicatedUsing = OnRep_MaxInventoryAmmoUpdate, Category = "Ammo", EditAnywhere, BlueprintReadOnly)
		int32 MaxInventoryAmmo = 100;

	//Total Ammo in the Weapon's Inventory.
	UPROPERTY(ReplicatedUsing = OnRep_InventoryAmmoUpdate, Category = "Ammo", EditAnywhere, BlueprintReadWrite)
		int32 InventoryAmmo;

	//Current Ammo in the Weapon's Magazine (Most important!).
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmoUpdate, Category = "Ammo", EditAnywhere, BlueprintReadWrite)
		int32 CurrentAmmo;

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

	UFUNCTION(NetMulticast, Reliable)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void Dequip();
	void Dequip_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY()
		FOnWeaponSpawned OnWeaponSpawned;

	UPROPERTY()
		FOnWeaponPickup OnWeaponPickup;
};
