

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WeaponData.h"

#include "WeaponObject.generated.h"

UCLASS(Blueprintable, ClassGroup = "Weapon System", meta = (BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API AWeaponObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponObject();

	UPROPERTY(Category = "Weapon Data", EditAnywhere, BlueprintReadWrite)
	UWeaponData* WeaponData;

	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadOnly)
	uint8 MaxAmmo;
	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
	uint8 CurrentAmmo;

	UPROPERTY(Category = "Visuals", VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WeaponModel;
protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


};
