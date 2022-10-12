

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

	UPROPERTY(ReplicatedUsing= OnRep_WeaponData, Category = "Weapon Data", EditAnywhere, BlueprintReadWrite)
		UWeaponData* WeaponData;

	UPROPERTY(Category = "Visuals", VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* WeaponModel;

	//Local Ammo will be updated after server callback. To reduce snappy shooting behaviour.
	uint8 LocalCurrentAmmo;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:
	UPROPERTY(replicated, Category = "Ammo", EditAnywhere, BlueprintReadOnly)
		uint8 MagazineSize = 10;
	UPROPERTY(replicated, Category = "Ammo", EditAnywhere, BlueprintReadOnly)
		uint8 MaxAmmo = 100;
	UPROPERTY(ReplicatedUsing= OnRep_AmmoUpdate, Category = "Ammo", EditAnywhere, BlueprintReadWrite)
		uint8 CurrentAmmo = 10;

	UFUNCTION()
		void OnRep_WeaponData();
	UFUNCTION()
		void OnRep_MagazineSizeUpdate();
	UFUNCTION()
		void OnRep_MaxAmmoUpdate();
	UFUNCTION()
		void OnRep_AmmoUpdate();

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
