#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"



#include "PlayerWeaponComponent.generated.h"

class APlayerPawn;

/**
 * 
 */

UCLASS(Blueprintable, ClassGroup = "Weapon System", meta = (BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API UPlayerWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//Equipped Weapon Data
	UPROPERTY(Category = "Weapons|Equipped Weapon Data", EditAnywhere, BlueprintReadWrite)
		UWeaponData* EquippedWeapon;

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE class UWeaponData* GetEquippedWeapon() const { return EquippedWeapon;

	//Weapon Switching
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "Set Equipped Weapon"), Category = "WeaponFunctions")
	void SetEquippedWeapon(UPARAM(ref) UWeaponData* NewWeapon);

	//Weapon Functionality
	UFUNCTION(Server, reliable, WithValidation, 
		BlueprintCallable, meta = (DisplayName = "Server Fire Weapon"), Category = "WeaponFunctions")
	void Server_FireWeapon();

	void Server_FireWeapon_Implementation();
	bool Server_FireWeapon_Validate();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reload Weapon"), Category = "WeaponFunctions")
	void ReloadWeapon();
};


