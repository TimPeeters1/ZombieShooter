


#include "PlayerPawn.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Init Camera
	FP_PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	FP_PlayerCamera->SetupAttachment(RootComponent);
	FP_PlayerCamera->SetRelativeLocation(FVector(0, 0, 65.0f));
	FP_PlayerCamera->bUsePawnControlRotation = true;

	//Init SpringArm and Set Variables to act as WeaponSway Component.
	FP_WeaponSway = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	FP_WeaponSway->SetupAttachment(FP_PlayerCamera);
	FP_WeaponSway->TargetArmLength = (float)0;
	FP_WeaponSway->bDoCollisionTest = false;
	FP_WeaponSway->bInheritRoll = false;
	FP_WeaponSway->bEnableCameraRotationLag = true;
	FP_WeaponSway->CameraRotationLagSpeed = (float)30;

	FP_ArmModel = CreateDefaultSubobject<USkeletalMeshComponent>("ArmModel");
	FP_ArmModel->SetupAttachment(FP_WeaponSway);

	FP_WeaponModel = CreateDefaultSubobject<UStaticMeshComponent>("WeaponModel");
	FP_WeaponModel->SetupAttachment(FP_ArmModel, TEXT("GripPoint"));

	PlayerWeaponComponent = CreateDefaultSubobject<UPlayerWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (this->IsLocallyControlled()) 
		GetMesh()->SetVisibility(false, true);
	else
		FP_ArmModel->SetVisibility(false, true);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement
	PlayerInputComponent->BindAxis("MoveX", this, &APlayerPawn::Move_XAxis);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerPawn::Move_YAxis);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerPawn::Turn_Character);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerPawn::Look_Up);

	//Weapon Component
	PlayerInputComponent->BindAction("Fire", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::Server_FireWeapon);
}

void APlayerPawn::Move_XAxis(float AxisValue) {
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void APlayerPawn::Move_YAxis(float AxisValue) {
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerPawn::Turn_Character(float AxisValue) {

	AddControllerYawInput(AxisValue * Turn_Rate * FApp::GetDeltaTime());
}

void APlayerPawn::Look_Up(float AxisValue) {
	AddControllerPitchInput(AxisValue * LookUp_Rate * FApp::GetDeltaTime());
}