#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MetasoundSource.h"
#include "Camera/CameraShakeBase.h"

#include "WeaponData.generated.h"

class UNiagaraSystem;

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	MELEE = 0 UMETA(DisplayName = "Melee"),
	SHOTGUN = 1  UMETA(DisplayName = "Shotgun"),
	SINGLEFIRE = 2     UMETA(DisplayName = "SingleFire"),
	BURSTFIRE = 3   UMETA(DisplayName = "BurstFire"),
	AUTOFIRE = 4     UMETA(DisplayName = "AutomaticFire")
};

/**
 *
 */
UCLASS(BlueprintType)
class UWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponName;

	UPROPERTY(Category = "Visuals|Meshes", EditAnywhere, BlueprintReadWrite)
		UStaticMesh* WeaponModel;
	UPROPERTY(Category = "Visuals|Meshes", EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* WeaponArmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform FP_Model_Transform;

	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
		uint8 Weapon_Default_MagazineSize;

	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
		uint8 Weapon_Default_MaxAmmo;

	UPROPERTY(Category = "Behaviour", EditAnywhere, BlueprintReadWrite)
		EWeaponType WeaponBehaviour;

	//Behaviour Settings
	UPROPERTY(Category = "Behaviour|Settings", EditAnywhere, BlueprintReadWrite)
		float Default_ShotDelay = 0.1f; 
	UPROPERTY(Category = "Behaviour|Settings", EditAnywhere, BlueprintReadWrite)
		float Default_ReloadTime = 1.0f;


	UPROPERTY(Category = "Visuals|CameraShake", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UCameraShakeBase> Shot_CamShake;

	UPROPERTY(Category = "Visuals|VFX", EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* MuzzleFlash_VFX;
	UPROPERTY(Category = "Visuals|Audio", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* ShotAudio;
	UPROPERTY(Category = "Visuals|Audio", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* ReloadAudio;
};
