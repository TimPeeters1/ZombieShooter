#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

#include "InteractableObjectInterface.h"
#include "RepairItem.h"

#include "PlayerWeaponComponent.h"
#include "GenericHealthComponent.h"

#include "PlayerPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionStop);

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API APlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInteraction")
		float PlayerInteractionRange = 450.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Turn_Rate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float LookUp_Rate = 45.0f;

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

	//Refrence to the Interactable Actor the Player is currently looking at. (InteractionTrace)
	UPROPERTY(Category = "Interaction", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* InteractingActor;

	UPROPERTY(Category = "Interaction|Inventory", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ARepairItem*> RepairObjectInventory;

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	virtual void BeginPlay() override;

	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);

	void Turn_Character(float AxisValue);
	void Look_Up(float AxisValue);

	void OnStartJump();
	void OnStopJump();

	void OnPerformInteraction();

	UFUNCTION(Server, reliable)
	void ServerPerformInteraction();
	void ServerPerformInteraction_Implementation();

	void InteractionTrace();

	UFUNCTION(Server, reliable)
	void ServerSetInteractingActor(AActor* InteractingObject);
	void ServerSetInteractingActor_Implementation(AActor* InteractingObject);

	UPROPERTY(BlueprintAssignable)
	FOnInteractionStart OnStartInteraction;

	UPROPERTY(BlueprintAssignable)
	FOnInteractionStop OnStopInteraction;

public:
	class UCameraComponent* GetFP_Camera() { return FP_PlayerCamera; }

	/** Returns Current FP Weapon Model **/
	FORCEINLINE class UStaticMeshComponent* GetWeaponModel() const { return FP_WeaponModel; }
	/** Returns  FP Audio Component **/
	FORCEINLINE class UAudioComponent* GetWeaponAudioComponent() const { return FP_WeaponAudio; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
