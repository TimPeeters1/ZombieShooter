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

UCLASS(BlueprintType)
class UWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponName;	

	UPROPERTY(Category = "Visuals|Meshes", EditAnywhere, BlueprintReadWrite)
		UStaticMesh* WeaponMesh;

	UPROPERTY(Category = "Visuals|Meshes", EditAnywhere, BlueprintReadWrite)
		FTransform WeaponMesh_Offset_FP;
	UPROPERTY(Category = "Visuals|Meshes", EditAnywhere, BlueprintReadWrite)
		FTransform WeaponMesh_Offset_TPS;

	//Behaviour Settings
	UPROPERTY(Category = "Behaviour", EditAnywhere, BlueprintReadWrite)
		EWeaponType WeaponBehaviour;
	UPROPERTY(Category = "Behaviour|Settings", EditAnywhere, BlueprintReadWrite)
		float ShotInterval = 0.1f; 
	UPROPERTY(Category = "Behaviour|Settings", EditAnywhere, BlueprintReadWrite)
		float ReloadTime = 1.0f;
	UPROPERTY(Category = "Behaviour|Settings", EditAnywhere, BlueprintReadWrite)
		float Weapon_Range = 3000.f;
	UPROPERTY(Category = "Behaviour|Settings", EditAnywhere, BlueprintReadWrite)
		float Weapon_Damage = 10.f;

	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
		int32 MagazineSize;
	UPROPERTY(Category = "Ammo", EditAnywhere, BlueprintReadWrite)
		int32 MaxInventoryAmmo;

	//Animations FP
	UPROPERTY(Category = "Visuals|Animations|FirstPerson", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* IdleAnimation_FP;
	UPROPERTY(Category = "Visuals|Animations|FirstPerson", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* AttackAnimation_FP;
	UPROPERTY(Category = "Visuals|Animations|FirstPerson", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* ReloadAnimation_FP;
	UPROPERTY(Category = "Visuals|Animations|FirstPerson", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* EquipAnimation_FP;

	//Animations TPS
	UPROPERTY(Category = "Visuals|Animations|ThirdPerson", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* IdleAnimation_TPS;
	UPROPERTY(Category = "Visuals|Animations|ThirdPerson", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* AttackAnimation_TPS;
	UPROPERTY(Category = "Visuals|Animations|ThirdPerson", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* ReloadAnimation_TPS;
	UPROPERTY(Category = "Visuals|Animations|ThirdPerson", EditAnywhere, BlueprintReadWrite)
		UAnimSequenceBase* EquipAnimation_TPS;

	//Camera Effects
	UPROPERTY(Category = "Visuals", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UCameraShakeBase> Shot_CamShake;
	UPROPERTY(Category = "Visuals", EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* MuzzleFlash_FX;

	//Audio Files
	UPROPERTY(Category = "Audio|FirstPerson", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* ShotAudio_FP;
	UPROPERTY(Category = "Audio|FirstPerson", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* MeleeHitAudio_FP;
	UPROPERTY(Category = "Audio|FirstPerson", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* ReloadAudio_FP;

	UPROPERTY(Category = "Audio|ThirdPerson", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* ShotAudio_TPS;
	UPROPERTY(Category = "Audio|ThirdPerson", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* MeleeHitAudio_TPS;
	UPROPERTY(Category = "Audio|ThirdPerson", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* ReloadAudio_TPS;
};
