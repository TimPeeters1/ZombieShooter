

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"
#include "GenericTeamAgentInterface.h"

#include "PlayerWeaponComponent.h"
#include "GenericHealthComponent.h"

#include "PlayerPawn.generated.h"


UCLASS()
class ZOMBIESHOOTER_API APlayerPawn : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Turn_Rate = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float LookUp_Rate = 45;

protected:

	//First Person Camera
	UPROPERTY(Category = "Components|Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FP_PlayerCamera;

	//Camera Arm for Simple Weapon Sway.
	UPROPERTY(Category = "Components|Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* FP_WeaponSway;

	//First Person Arms Model
	UPROPERTY(Category = "Components|Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* FP_ArmModel;

	UPROPERTY(Category = "Components|Weapons", VisibleDefaultsOnly , BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* FP_WeaponModel;

	UPROPERTY(Category = "Components|Weapons", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAudioComponent* FP_WeaponAudio;

	/*Health*/
	//Health Component
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UGenericHealthComponent* HealthComponent;

	/*Weapon Handling*/
	//Weapon Logic Component
	UPROPERTY(replicated, Category = "Components|Weapons", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UPlayerWeaponComponent* PlayerWeaponComponent;

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	FGenericTeamId TeamId = 0;

	virtual void BeginPlay() override;

	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);

	void Turn_Character(float AxisValue);
	void Look_Up(float AxisValue);

public:
	/** Returns Current FP Weapon Model **/
	FORCEINLINE class UStaticMeshComponent* GetWeaponModel() const { return FP_WeaponModel; }
	/** Returns  FP Audio Component **/
	FORCEINLINE class UAudioComponent* GetWeaponAudioComponent() const { return FP_WeaponAudio; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
};
