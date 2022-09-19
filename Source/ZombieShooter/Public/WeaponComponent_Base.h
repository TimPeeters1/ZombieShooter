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

public:
	//Equipped Weapon Data
	UPROPERTY(Category = "Weapons|Equipped Weapon Data", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWeaponData* EquippedWeapon;
	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE class UWeaponData* GetEquippedWeapon() const { return EquippedWeapon; }

	UFUNCTION(BlueprintCallable, meta=(DisplayName = "Set Equipped Weapon"), Category = "WeaponFunctions")
	void SetEquippedWeapon(UPARAM(ref) UWeaponData* NewWeapon);
};
