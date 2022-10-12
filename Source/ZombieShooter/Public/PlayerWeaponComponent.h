#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#include "WeaponObject.h"

#include "PlayerWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadEvent);

class APlayerPawn;

/**
 * 
 */

UCLASS(Blueprintable, ClassGroup = "Weapon System", meta = (BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API UPlayerWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerWeaponComponent();

protected:
	//StartingWeapons
	UPROPERTY(Category = "Weapons|Equipped Weapons ", EditAnywhere, BlueprintReadWrite)
	TArray<UWeaponData*> StartingWeapons;

	//Equipped Weapon Data
	UPROPERTY(Replicated, Category = "Weapons|Equipped Weapons", EditInstanceOnly, BlueprintReadOnly)
	TArray<AWeaponObject*> EquippedWeapons;

	//Equipped Weapon Data
	UPROPERTY(Replicated, Category = "Weapons|Equipped Weapons", EditInstanceOnly, BlueprintReadOnly)
	AWeaponObject* ActiveWeapon;

public:
	UPROPERTY(BlueprintAssignable)
		FOnFireEvent OnFireEvent;
	UPROPERTY(BlueprintAssignable)
		FOnReloadEvent OnReloadEvent; 
	
	UFUNCTION(Server, reliable)
	void SpawnStartWeapons();
	void SpawnStartWeapons_Implementation();

	UFUNCTION(NetMulticast, reliable, BlueprintCallable, meta = (DisplayName = "Set Equipped Weapon"), Category = "WeaponFunctions")
	void SetEquippedWeapon(uint8 Index);
	void SetEquippedWeapon_Implementation(uint8 Index);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Equip Primary Weapon"), Category = "WeaponFunctions")
	void EquipPrimaryWeapon();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Equip Secondary Weapon"), Category = "WeaponFunctions")
	void EquipSecondaryWeapon();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fire Weapon"), Category = "WeaponFunctions")
	void OnFire();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reload Weapon"), Category = "WeaponFunctions")
	void ReloadWeapon();

	//Weapon Functionality
	UFUNCTION(Server, reliable)
	void ClientFireWeapon();
	void ClientFireWeapon_Implementation();

	UFUNCTION(NetMulticast, reliable)
	void Server_FireWeapon();
	void Server_FireWeapon_Implementation();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};


