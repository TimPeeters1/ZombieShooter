#pragma once

//Unreal Includes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

//Character Components
#include "Characters/Player/InteractableObjectInterface.h"
#include "Characters/Player/Weapons/PlayerWeaponComponent.h"
#include "Characters/Player/PlayerInventoryComponent.h"
#include "Characters/GenericHealthComponent.h"

//Other
#include "GameObjective/RepairObject.h"

#include "PlayerPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionStop);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeathLocal);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxWalkSpeed = 450.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxSprintSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxCrouchSpeed = 300.0f;

	UPROPERTY(Category = "Movement", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bSprinting;
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

	/*Weapons*/
	//Weapon Logic Component
	UPROPERTY(replicated, Category = "Components|Weapons", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UPlayerWeaponComponent* PlayerWeaponComponent;

	/*Inventory*/
	//Inventory Component
	UPROPERTY(replicated, Category = "Components|Inventory", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UPlayerInventoryComponent* InventoryComponent;

	//Refrence to the Interactable Actor the Player is currently looking at. (InteractionTrace)
	UPROPERTY(Category = "Interaction", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* InteractingActor;

	/*TODO Replace with variable in Inventory Component*/
	UPROPERTY(Category = "Components|Inventory", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ARepairObject*> RepairObjectInventory;


public:
	void OnSetPlayerViewMode();

	/** Returns Current FP Camera **/
	class UCameraComponent* GetFP_Camera() { return FP_PlayerCamera; }
	/** Returns Current FP Arm Model **/
	 class USkeletalMeshComponent* GetArmModel() { return FP_ArmModel; }
	/** Returns Current FP Weapon Model **/
	 class UStaticMeshComponent* GetWeaponModel()  { return FP_WeaponModel; }
	/** Returns FP Audio Component **/
	 class UAudioComponent* GetWeaponAudioComponent()  { return FP_WeaponAudio; }

	 /** Returns Player Health Component **/
	 class UGenericHealthComponent* GetHealthComponent() { return HealthComponent; }
	 /** Returns Player Weapon Component **/
	 class UPlayerWeaponComponent* GetWeaponComponent() { return PlayerWeaponComponent; }
	 /** Returns Player Inventory Component **/
	 class UPlayerInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION()
	void OnDeath();

	/*
	 * Input Logic
	 */
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);

	void Turn_Character(float AxisValue);
	void Look_Up(float AxisValue);

	void OnStartJump();
	void OnStopJump();

	void OnStartSprint();
	void OnStopSprint();

	UFUNCTION(Server, Reliable)
	void OnStartSprint_Server();
	void OnStartSprint_Server_Implementation();

	UFUNCTION(Server, Reliable)
	void OnStopSprint_Server();
	void OnStopSprint_Server_Implementation();


	/*
	 * Interaction Logic
	 * (Could be moved to actor component in future!)
	 */
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

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeathLocal OnPlayerDeathLocal;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeathReplicated;

};
