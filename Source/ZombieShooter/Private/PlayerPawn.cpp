
#include "PlayerPawn.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	if (!FP_PlayerCamera)
	//Init Camera
	FP_PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	FP_PlayerCamera->SetupAttachment(RootComponent);
	FP_PlayerCamera->SetRelativeLocation(FVector(0, 0, 65.0f));
	FP_PlayerCamera->bUsePawnControlRotation = true;

	if (!FP_WeaponSway)
	//Init SpringArm and Set Variables to act as WeaponSway Component.
	FP_WeaponSway = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	FP_WeaponSway->SetupAttachment(FP_PlayerCamera);
	FP_WeaponSway->TargetArmLength = (float)0;
	FP_WeaponSway->bDoCollisionTest = false;
	FP_WeaponSway->bInheritRoll = false;
	FP_WeaponSway->bEnableCameraRotationLag = true;
	FP_WeaponSway->CameraRotationLagSpeed = (float)30;

	if (!FP_ArmModel)
	FP_ArmModel = CreateDefaultSubobject<USkeletalMeshComponent>("ArmModel");
	FP_ArmModel->SetupAttachment(FP_WeaponSway);

	if (!FP_WeaponModel)
	FP_WeaponModel = CreateDefaultSubobject<UStaticMeshComponent>("WeaponModel");
	FP_WeaponModel->SetupAttachment(FP_ArmModel, TEXT("GripPoint"));

	if (!FP_WeaponAudio)
	FP_WeaponAudio = CreateDefaultSubobject<UAudioComponent>("WeaponAudioComponent");
	FP_WeaponAudio->SetupAttachment(FP_WeaponModel);

	//if(!PlayerWeaponComponent)
	//PlayerWeaponComponent = CreateDefaultSubobject<UPlayerWeaponComponent>("WeaponComponent");

	if(!HealthComponent)
	HealthComponent = CreateDefaultSubobject<UGenericHealthComponent>("HealthComponent");
}

void APlayerPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerPawn, PlayerWeaponComponent);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (!PlayerWeaponComponent)
		PlayerWeaponComponent = (UPlayerWeaponComponent*) GetComponentByClass(TSubclassOf<UPlayerWeaponComponent>());

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
	PlayerInputComponent->BindAction("Fire", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::OnFire);
	PlayerInputComponent->BindAction("EquipPrimaryWeapon", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::EquipPrimaryWeapon);
	PlayerInputComponent->BindAction("EquipSecondaryWeapon", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::EquipSecondaryWeapon);
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

float APlayerPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HealthComponent->ReduceHealth(DamageAmount);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}



