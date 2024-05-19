#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#include "WeaponObject.h"

#include "PlayerWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireEvent_FirstPerson);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireEvent_ThirdPerson);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadEvent_FirstPerson);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadEvent_ThirdPerson);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwitchWeapon_FirstPerson);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwitchWeapon_ThirdPerson);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeWeaponInventory);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponHit);

class APlayerPawn;

UCLASS(Blueprintable, ClassGroup = "Weapon System", meta = (BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API UPlayerWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerWeaponComponent();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		bool bIsFiring;
protected:
	//StartingWeapons
	UPROPERTY(Category = "Weapons|Equipped Weapons ", EditAnywhere, BlueprintReadWrite)
		TArray<UWeaponData*> StartingWeapons;

	//Equipped Weapon Data
	UPROPERTY(Category = "Weapons|Equipped Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		uint8 Max_EquippedWeapon_Count = 2;

	//Equipped Weapon Data
	UPROPERTY(ReplicatedUsing = OnRep_InventoryWeapons, Category = "Weapons|Equipped Weapons", EditInstanceOnly, BlueprintReadOnly)
		TArray<AWeaponObject*> InventoryWeapons;

	//Equipped Weapon Data
	UPROPERTY(ReplicatedUsing = OnRep_ActiveWeapon, Category = "Weapons|Equipped Weapons", EditInstanceOnly, BlueprintReadOnly)
		AWeaponObject* ActiveWeapon;

	AWeaponObject* SpawnWeaponObject(UWeaponData* WeaponData);

	void SpawnStartWeapons();

	void BlockFire();
	void UnBlockFire();

	bool bFireBlocked;
	bool bReloading;

	FTimerHandle AutomaticFireTimer;
	FTimerHandle FireDelayTimer;
	FTimerHandle ReloadTimer;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:

	/** Returns Equipped WeaponObjects**/
	TArray<AWeaponObject*> GetEquippedWeapons() const { return InventoryWeapons; }

	/** Returns Active WeaponObject**/
	AWeaponObject* GetActiveWeapon() const { return ActiveWeapon; }

	/*
	* Input Functions (bound in APlayerPawn)
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Equip Primary Weapon"), Category = "WeaponFunctions")
		void EquipPrimaryWeapon();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Equip Secondary Weapon"), Category = "WeaponFunctions")
		void EquipSecondaryWeapon();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnStart"), Category = "WeaponFunctions")
		void OnFire();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnFireEnd"), Category = "WeaponFunctions")
		void OnFireEnd();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnReload"), Category = "WeaponFunctions")
		void OnReload();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SingleFire"), Category = "WeaponFunctions")
		void SingleFire();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MeleeFire"), Category = "WeaponFunctions")
		void MeleeFire();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ReloadWeapon"), Category = "WeaponFunctions")
		void ReloadWeapon();

	UPROPERTY(ReplicatedUsing = OnRep_HitResult, BlueprintReadOnly, VisibleAnywhere)
	FHitResult LastHitResult;

	/*
	* RPC Implementation Client->Server
	*/
	UFUNCTION(Server, Reliable)
		void OnStartMelee();
	void OnStartMelee_Implementation();

	UFUNCTION(Server, Reliable)
		void SetEquippedWeapon_Request(uint8 Index);
	void SetEquippedWeapon_Request_Implementation(uint8 Index);

	UFUNCTION(Server, reliable)
		void ServerFireWeapon();
	void ServerFireWeapon_Implementation();

	UFUNCTION(Server, reliable)
		void ServerMeleeWeapon();
	void ServerMeleeWeapon_Implementation();

	UFUNCTION(Server, reliable)
		void ServerReloadWeapon();
	void ServerReloadWeapon_Implementation();

	UFUNCTION(Server, Reliable)
		void DropFirstWeaponFromInventory();
	void DropFirstWeaponFromInventory_Implementation();

	/*
	* RPC Implementation Server->All Clients
	*/
	UFUNCTION(NetMulticast, Reliable)
		void FireCallback();
	void FireCallback_Implementation();

	UFUNCTION(Server, Reliable)
		void UpdateEquippedWeapons();
	void UpdateEquippedWeapons_Implementation();


	/*
	* Replication Notifies
	*/
	UFUNCTION()
		void OnRep_ActiveWeapon();

	UFUNCTION()
		void OnRep_InventoryWeapons();

	UFUNCTION()
		void OnRep_HitResult();

	/*
	* Inventory Function
	*/
	UFUNCTION()
		void OnPickupWeapon(AWeaponObject* WeaponToPickup);

	/*
	* First Person Event Delegates
	*/
	UPROPERTY(BlueprintAssignable)
		FOnFireEvent_FirstPerson OnFireEvent_FP;
	UPROPERTY(BlueprintAssignable)
		FOnReloadEvent_FirstPerson OnReloadEvent_FP;
	UPROPERTY(BlueprintAssignable)
		FOnSwitchWeapon_FirstPerson OnSwitchWeapon_FP;
	UPROPERTY(BlueprintAssignable)
		FOnChangeWeaponInventory OnWeaponInventoryChanged;

	/*
	* Third Person Event Delegates
	*/
	UPROPERTY(BlueprintAssignable)
		FOnFireEvent_ThirdPerson OnFireEvent_TPS;
	UPROPERTY(BlueprintAssignable)
		FOnReloadEvent_ThirdPerson OnReloadEvent_TPS;
	UPROPERTY(BlueprintAssignable)
		FOnSwitchWeapon_ThirdPerson OnSwitchWeapon_TPS;

	//Hit Object Callback
	UPROPERTY(BlueprintAssignable)
		FOnWeaponHit OnWeaponHit;
};


