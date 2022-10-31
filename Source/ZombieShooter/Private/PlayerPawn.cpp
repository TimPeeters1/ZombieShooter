
#include "PlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "RepairObjective.h"



// Sets default values
APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	if (GetCapsuleComponent())
		GetCapsuleComponent()->SetCapsuleHalfHeight(100.0f);

	if (!FP_PlayerCamera) {
		//Init Camera
		FP_PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
		FP_PlayerCamera->SetupAttachment(RootComponent);
		FP_PlayerCamera->SetRelativeLocation(FVector(0, 0, 75.0f));
		FP_PlayerCamera->bUsePawnControlRotation = true;
	}

	if (!FP_WeaponSway) {
		//Init SpringArm and Set Variables to act as WeaponSway Component.
		FP_WeaponSway = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
		FP_WeaponSway->SetupAttachment(FP_PlayerCamera);
		FP_WeaponSway->TargetArmLength = (float)0;
		FP_WeaponSway->bDoCollisionTest = false;
		FP_WeaponSway->bInheritRoll = false;
		FP_WeaponSway->bEnableCameraRotationLag = true;
		FP_WeaponSway->CameraRotationLagSpeed = (float)30;
	}

	if (!FP_ArmModel) {
		FP_ArmModel = CreateDefaultSubobject<USkeletalMeshComponent>("ArmModel");
		FP_ArmModel->SetupAttachment(FP_WeaponSway);
		FP_ArmModel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (!FP_WeaponModel) {
		FP_WeaponModel = CreateDefaultSubobject<UStaticMeshComponent>("WeaponModel");
		FP_WeaponModel->SetupAttachment(FP_ArmModel, TEXT("GripPoint"));
		FP_WeaponModel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (!FP_WeaponAudio) {
		FP_WeaponAudio = CreateDefaultSubobject<UAudioComponent>("WeaponAudioComponent");
		FP_WeaponAudio->SetupAttachment(FP_WeaponModel);
		FP_WeaponAudio->SetAutoActivate(false);
	}

	if (!PlayerWeaponComponent) {
		PlayerWeaponComponent = CreateDefaultSubobject<UPlayerWeaponComponent>("WeaponComponent");
		PlayerWeaponComponent->SetIsReplicated(true);
	}

	if (!HealthComponent) {
		HealthComponent = CreateDefaultSubobject<UGenericHealthComponent>("HealthComponent");
		HealthComponent->SetIsReplicated(true);
	}
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

	if (IsLocallyControlled())
		GetMesh()->SetVisibility(false, true);
	else
		FP_ArmModel->SetVisibility(false, true);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractionTrace();
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerPawn::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerPawn::OnStopJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerPawn::OnPerformInteraction);

	if (!PlayerWeaponComponent) return;
	//Weapon Component
	PlayerInputComponent->BindAction("Fire", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::OnFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::OnReloadWeapon);
	PlayerInputComponent->BindAction("EquipPrimaryWeapon", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::EquipPrimaryWeapon);
	PlayerInputComponent->BindAction("EquipSecondaryWeapon", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::EquipSecondaryWeapon);
}

void APlayerPawn::Move_XAxis(float AxisValue) {
	if (!IsLocallyControlled()) return;
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void APlayerPawn::Move_YAxis(float AxisValue) {
	if (!IsLocallyControlled()) return;
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerPawn::Turn_Character(float AxisValue)
{
	if (!IsLocallyControlled()) return;
	AddControllerYawInput(AxisValue * Turn_Rate * FApp::GetDeltaTime());
}

void APlayerPawn::Look_Up(float AxisValue)
{
	if (!IsLocallyControlled()) return;
	AddControllerPitchInput(AxisValue * LookUp_Rate * FApp::GetDeltaTime());
}

void APlayerPawn::OnStartJump()
{
	if (!IsLocallyControlled()) return;
	Jump();
}

void APlayerPawn::OnStopJump()
{
	if (!IsLocallyControlled()) return;
	StopJumping();
}

void APlayerPawn::OnPerformInteraction()
{
	if (!IsLocallyControlled()) return;
	if (InteractingActor) {
		Cast<IInteractableObjectInterface>(InteractingActor)->Execute_OnInteract(InteractingActor);

		//TEMP Implemenation for Object Pickups.
		if (Cast<ARepairItem>(InteractingActor)) {
			ARepairItem* RepairObject = Cast<ARepairItem>(InteractingActor);
			if (!RepairObjectInventory.Contains(RepairObject)) {
				RepairObjectInventory.Add(RepairObject);
			}
		} else if (Cast<ARepairObjective>(InteractingActor)) {
			ARepairObjective* RepairObjective = Cast<ARepairObjective>(InteractingActor);
			if (!RepairObjectInventory.IsEmpty()) {
				RepairObjective->AddRepairItem(RepairObjectInventory[0]);
				RepairObjectInventory.RemoveAt(0);
			}
		}
	}
}

void APlayerPawn::InteractionTrace()
{
	//Only performs locally as this trace is fired every frame. Callback will be implemented further down the line.
	if (!IsLocallyControlled()) return;

	if (FP_PlayerCamera) {
		FVector StartLoc = FP_PlayerCamera->GetComponentLocation();
		FVector ControlRot = GetControlRotation().Vector() * PlayerInteractionRange;
		FVector EndLoc = StartLoc + ControlRot;

		FHitResult HitResult(ForceInit);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor(255, 0, 0), false, 2.0f, 0, 3.f);

		GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, CollisionParams);

		if (HitResult.GetActor()) {
			IInteractableObjectInterface* NewInteractor = Cast<IInteractableObjectInterface>(HitResult.GetActor());
			if (NewInteractor) {
				if (HitResult.GetActor() != InteractingActor) {
					//Set New Interacting Actor
					InteractingActor = HitResult.GetActor();
					Cast<IInteractableObjectInterface>(InteractingActor)->Execute_StartHover(InteractingActor);
					OnStartInteraction.Broadcast();
				}
			}

			//TODO REFACTOR DOUBLE STATEMENTS, this is stupid.
			else {
				if (InteractingActor) {
					Cast<IInteractableObjectInterface>(InteractingActor)->Execute_StopHover(InteractingActor);
					OnStopInteraction.Broadcast();
				}
				InteractingActor = nullptr;
			}
		}
		else {
			if (InteractingActor) {
				Cast<IInteractableObjectInterface>(InteractingActor)->Execute_StopHover(InteractingActor);
				OnStopInteraction.Broadcast();
			}
			InteractingActor = nullptr;
		}
	}

}

float APlayerPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
		HealthComponent->ReduceHealth(DamageAmount);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}



