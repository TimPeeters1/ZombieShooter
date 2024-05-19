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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeathLocal);

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API APlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerPawn();

	UPROPERTY(ReplicatedUsing = OnRep_PlayerColor, BlueprintReadOnly, VisibleAnywhere)
		FColor PlayerGameColor;

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

	UPROPERTY(Replicated, Category = "Movement", VisibleAnywhere, BlueprintReadOnly)
	bool bSprinting;
	
	UPROPERTY(Replicated, Category = "Movement", VisibleAnywhere, BlueprintReadOnly)
	FRotator CameraRotation;
	
protected:

	//First Person Camera
	UPROPERTY(Replicated, Category = "Components|Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FP_PlayerCamera;

	//Camera Arm for Simple Weapon Sway.
	UPROPERTY(Category = "Components|FirstPerson", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* FP_WeaponSway;

	//First Person Arms Model
	UPROPERTY(Category = "Components|FirstPerson", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* ArmModel_FP;
	UPROPERTY(Category = "Components|FirstPerson", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* WeaponModel_FP;

	//Third Person Model
	UPROPERTY(Category = "Components|ThirdPerson", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* WeaponModel_TPS;

	/*Health*/
	//Health Component
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UGenericHealthComponent* HealthComponent;

	/*Weapons*/
	//Weapon Logic Component
	UPROPERTY(replicated, Category = "Components|Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UPlayerWeaponComponent* WeaponComponent;

	/*Inventory*/
	//Inventory Component
	UPROPERTY(Replicated, Category = "Components|Inventory", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UPlayerInventoryComponent* InventoryComponent;

	//Refrence to the Interactable Actor the Player is currently looking at. (InteractionTrace)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	AActor* InteractingActor;


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

	//Jump Functionality
	void OnStartJump();
	void OnStopJump();
	
	//Crouch Functionality
	void OnStartCrouch();
	void OnStopCrouch();

	//Sprint Functionality
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
	void OnStartInteraction();
	void OnStopInteraction();

	UFUNCTION(Server, reliable)
	void ServerStartInteraction();
	void ServerStartInteraction_Implementation();

	UFUNCTION(Server, reliable)
	void ServerStopInteraction();
	void ServerStopInteraction_Implementation();

	void InteractionTrace();

	UFUNCTION(Server, reliable)
	void ServerSetInteractingActor(AActor* InteractingObject);
	void ServerSetInteractingActor_Implementation(AActor* InteractingObject);
	
	/*
	* Delgates exposed for use in blueprints.
	*/
	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeathLocal OnPlayerDeathLocal;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeathReplicated;

public:
	void OnSetPlayerViewMode();

	
	//Callback for IInteractableObjectInterface when starting hover
	UFUNCTION(BlueprintImplementableEvent)
		void OnStartHover(const FString& InteractionMessage);

	//Callback for IInteractableObjectInterface when stopping hover
	UFUNCTION(BlueprintImplementableEvent)
		void OnStopHover();

	//Callback for when PlayerColor is replicated via PlayerController
	UFUNCTION(BlueprintImplementableEvent)
		void OnRep_PlayerColor();

	/** Returns Current First Person Camera **/
	class UCameraComponent* GetFP_Camera() { return FP_PlayerCamera; }
	/** Returns Current First Person Arm Model **/
	 class USkeletalMeshComponent* GetFP_ArmModel() { return ArmModel_FP; }
	/** Returns Current First Person Weapon Model **/
	 class UStaticMeshComponent* GetFP_WeaponModel()  { return WeaponModel_FP; }


	 /** Returns Current Third Person Weapon Model **/
	 class UStaticMeshComponent* GetTPS_WeaponModel() { return WeaponModel_TPS; }

	 /** Returns Player Health Component **/
	 class UGenericHealthComponent* GetHealthComponent() { return HealthComponent; }
	 /** Returns Player Weapon Component **/
	 class UPlayerWeaponComponent* GetWeaponComponent() { return WeaponComponent; }
	 /** Returns Player Inventory Component **/
	 class UPlayerInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
