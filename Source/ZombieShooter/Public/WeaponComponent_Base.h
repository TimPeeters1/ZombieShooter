#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponData.h"

#include "WeaponComponent_Base.generated.h"
/**
 * 
 */

UCLASS(Blueprintable, ClassGroup = "Weapon System", meta = (BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API UWeaponComponent_Base : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "Weapons|Ammunition", EditAnywhere, BlueprintReadWrite)
		uint8 Ammo_Max;
	UPROPERTY(Category = "Weapons|Ammunition", VisibleAnywhere, BlueprintReadWrite)
		uint8 Ammo_Current;

public:
	//Equipped Weapon Data
	UPROPERTY(Category = "Weapons|Equipped Weapon Data", EditAnywhere, BlueprintReadWrite)
		UWeaponData* EquippedWeapon;

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE class UWeaponData* GetEquippedWeapon() const { return EquippedWeapon; }

	//Weapon Switching
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "Set Equipped Weapon"), Category = "WeaponFunctions")
	void SetEquippedWeapon(UPARAM(ref) UWeaponData* NewWeapon);

	//Weapon Functionality
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fire Weapon"), Category = "WeaponFunctions")
	void FireWeapon();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reloead Weapon"), Category = "WeaponFunctions")
	void ReloadWeapon();
};


