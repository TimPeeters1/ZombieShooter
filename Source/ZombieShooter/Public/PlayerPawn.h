

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerWeaponComponent.h"

#include "PlayerPawn.generated.h"


UCLASS()
class ZOMBIESHOOTER_API APlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerPawn();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Movement")
	float Turn_Rate = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LookUp_Rate = 45;

protected:

	//First Person Camera
	UPROPERTY(Category = "Character|First Person Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FP_PlayerCamera;

	//Camera Arm for Simple Weapon Sway.
	UPROPERTY(Category = "Character|First Person Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* FP_WeaponSway;

	//First Person Arms Model
	UPROPERTY(Category = "Character|First Person Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FP_ArmModel;

	UPROPERTY(Category = "Weapons|FP_WeaponModel", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FP_WeaponModel;

	/*Weapon Handling*/
	//Weapon Logic Component
	UPROPERTY(Category = "Weapons|Player Weapon Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPlayerWeaponComponent* PlayerWeaponComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);

	void Turn_Character(float AxisValue);
	void Look_Up(float AxisValue);

public:	
	/** Returns Current FP Weapon Model **/
	FORCEINLINE class UStaticMeshComponent* GetWeaponModel() const { return FP_WeaponModel; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
