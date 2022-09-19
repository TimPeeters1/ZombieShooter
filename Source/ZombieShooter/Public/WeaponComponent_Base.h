#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponData.h"
#include "WeaponComponent_Base.generated.h"


/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UWeaponComponent_Base : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	//Equipped Weapon Data
	//UPROPERTY(Category = "Weapons|Equipped Weapon Data", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//UWeaponData* EquippedWeapon;

public:
	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE class UWeaponData* GetEquippedWeapon() const { return EquippedWeapon; }

	//UFUNCTION(BlueprintCallable)
	//void SetEquippedWeapon(UWeaponData* NewWeapon);
};
