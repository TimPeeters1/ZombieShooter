

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(Category = "Weapon Data", EditAnywhere, BlueprintReadWrite)
	UWeaponData* WeaponData;

	UPROPERTY(replicated, Category = "Ammo", EditAnywhere, BlueprintReadOnly)
	uint8 MaxAmmo;
	UPROPERTY(replicated, Category = "Ammo", EditAnywhere, BlueprintReadWrite)
	uint8 CurrentAmmo;

	UPROPERTY(Category = "Visuals", VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WeaponModel;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Ammo"), Category = "WeaponFunctions|Ammo")
	uint8 SetAmmo(uint8 newAmount);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Max Ammo"), Category = "WeaponFunctions|Ammo")
	uint8 SetMaxAmmo(uint8 newAmount);

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
