
#include "Characters/Player/PlayerPawn.h"

#include "General/GameMode_Main.h"
#include "Characters/Player/PlayerDeathCamera.h"
#include "Components/CapsuleComponent.h"
#include "GameObjective/RepairGoal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	GetCapsuleComponent()->SetCapsuleHalfHeight(100.0f);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxCrouchSpeed;


	//Init Camera
	FP_PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	FP_PlayerCamera->SetupAttachment(RootComponent);
	FP_PlayerCamera->SetRelativeLocation(FVector(0, 0, 75.0f));
	FP_PlayerCamera->bUsePawnControlRotation = true;

	//Init SpringArm and Set Variables to act as WeaponSway Component.
	FP_WeaponSway = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	FP_WeaponSway->SetupAttachment(FP_PlayerCamera);
	FP_WeaponSway->TargetArmLength = (float)0;
	FP_WeaponSway->bDoCollisionTest = false;
	FP_WeaponSway->bInheritRoll = false;
	FP_WeaponSway->bEnableCameraRotationLag = true;
	FP_WeaponSway->CameraRotationLagSpeed = (float)30;

	ArmModel_FP = CreateDefaultSubobject<USkeletalMeshComponent>("ArmModel_FP");
	ArmModel_FP->SetupAttachment(FP_WeaponSway);
	ArmModel_FP->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponModel_FP = CreateDefaultSubobject<UStaticMeshComponent>("WeaponModel_FP");
	WeaponModel_FP->SetupAttachment(ArmModel_FP, TEXT("GripPoint"));
	WeaponModel_FP->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponModel_TPS = CreateDefaultSubobject<UStaticMeshComponent>("TPS_WeaponModel");
	WeaponModel_TPS->SetupAttachment(GetMesh(), TEXT("GripPoint"));
	WeaponModel_TPS->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponComponent = CreateDefaultSubobject<UPlayerWeaponComponent>("WeaponComponent");
	WeaponComponent->SetIsReplicated(true);

	HealthComponent = CreateDefaultSubobject<UGenericHealthComponent>("HealthComponent");
	HealthComponent->SetIsReplicated(true);

	InventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>("InventoryComponent");
	InventoryComponent->SetIsReplicated(true);
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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerPawn::OnStartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerPawn::OnStopSprint);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerPawn::OnStartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerPawn::OnStopCrouch);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerPawn::OnStartInteraction);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerPawn::OnStopInteraction);

	if (!WeaponComponent) return;
	//Weapon Component
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UPlayerWeaponComponent::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UPlayerWeaponComponent::OnFireEnd);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UPlayerWeaponComponent::OnReload);
	PlayerInputComponent->BindAction("EquipPrimaryWeapon", IE_Pressed, WeaponComponent, &UPlayerWeaponComponent::EquipPrimaryWeapon);
	PlayerInputComponent->BindAction("EquipSecondaryWeapon", IE_Pressed, WeaponComponent, &UPlayerWeaponComponent::EquipSecondaryWeapon);

	PlayerInputComponent->BindAction("DropWeapon", IE_Pressed, WeaponComponent, &UPlayerWeaponComponent::DropFirstWeaponFromInventory);

	if (!InventoryComponent) return;
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, InventoryComponent, &UPlayerInventoryComponent::DropFirstItemFromInventory);
}

void APlayerPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerPawn, WeaponComponent);
	DOREPLIFETIME(APlayerPawn, InventoryComponent);
	DOREPLIFETIME(APlayerPawn, PlayerGameColor);

	DOREPLIFETIME(APlayerPawn, bSprinting);
}


void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	OnSetPlayerViewMode();

	if (HealthComponent) {
		HealthComponent->OnDeath.AddUniqueDynamic(this, &APlayerPawn::OnDeath);
	}
}

void APlayerPawn::OnSetPlayerViewMode()
{
	if (IsLocallyControlled()) {
		GetMesh()->SetVisibility(false, false);
		if (WeaponModel_TPS) {
			WeaponModel_TPS->SetVisibility(false, true);
		}
		if (ArmModel_FP) {
			ArmModel_FP->SetVisibility(true, true);
		}
	}
	else {
		GetMesh()->SetVisibility(true, false);
		if (WeaponModel_TPS) {
			WeaponModel_TPS->SetVisibility(true, true);
		}
		if (ArmModel_FP) {
			ArmModel_FP->SetVisibility(false, true);
		}
	}
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractionTrace();
}

void APlayerPawn::Move_XAxis(float AxisValue) {
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void APlayerPawn::Move_YAxis(float AxisValue) {
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerPawn::Turn_Character(float AxisValue)
{
	AddControllerYawInput(AxisValue * Turn_Rate * FApp::GetDeltaTime());
}

void APlayerPawn::Look_Up(float AxisValue)
{
	AddControllerPitchInput(AxisValue * LookUp_Rate * FApp::GetDeltaTime());
}

void APlayerPawn::OnStartJump()
{
	Jump();
}

void APlayerPawn::OnStopJump()
{
	StopJumping();
}

void APlayerPawn::OnStartCrouch()
{
	Crouch();
	FVector MeshCrouchPosition = GetMesh()->GetRelativeLocation() + GetCharacterMovement()->GetCrouchedHalfHeight();
	GetMesh()->SetRelativeLocation(MeshCrouchPosition);
}

void APlayerPawn::OnStopCrouch()
{
	UnCrouch();
	FVector MeshCrouchPosition = GetMesh()->GetRelativeLocation() - GetCharacterMovement()->GetCrouchedHalfHeight();
	GetMesh()->SetRelativeLocation(MeshCrouchPosition);
}

void APlayerPawn::OnStartSprint()
{
	if (!bSprinting) {
		bSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
	}
	OnStartSprint_Server();
}

void APlayerPawn::OnStopSprint()
{
	if (bSprinting) {
		bSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
	OnStopSprint_Server();
}

void APlayerPawn::OnStartSprint_Server_Implementation()
{
	if (!bSprinting) {
		bSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
	}
}

void APlayerPawn::OnStopSprint_Server_Implementation()
{
	if (bSprinting) {
		bSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
}

void APlayerPawn::OnStartInteraction()
{
	if (!IsLocallyControlled()) return;

	ServerStartInteraction();

	//This Could be a little overdone, as this is also called from the Server RPC (ServerStartInteraction). 
	//but as lag would have it, the earlier the actor responds, the better.. right?
	if (InteractingActor && !UKismetSystemLibrary::IsServer(GetWorld()))
		Cast<IInteractableObjectInterface>(InteractingActor)->OnStartInteract(this);

}

void APlayerPawn::OnStopInteraction() {
	if (!IsLocallyControlled()) return;

	ServerStopInteraction();

	//This Could be a little overdone, as this is also called from the Server RPC (ServerStopInteraction). 
	//but as lag would have it, the earlier the actor responds, the better.. right?
	if (InteractingActor && !UKismetSystemLibrary::IsServer(GetWorld()))
		Cast<IInteractableObjectInterface>(InteractingActor)->OnStopInteract(this);
}

void APlayerPawn::ServerStartInteraction_Implementation()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (InteractingActor) {
		Cast<IInteractableObjectInterface>(InteractingActor)->OnStartInteract(this);
	}
}

void APlayerPawn::ServerStopInteraction_Implementation()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (InteractingActor) {
		Cast<IInteractableObjectInterface>(InteractingActor)->OnStopInteract(this);
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
					//Replicate InteractingActor to Server
					ServerSetInteractingActor(InteractingActor);

					//Start Local Hover FX
					Cast<IInteractableObjectInterface>(InteractingActor)->StartHover(this);
				}
			}
			else if (InteractingActor)
			{
				Cast<IInteractableObjectInterface>(InteractingActor)->StopHover(this);
				//OnStopInteraction.Broadcast();

				ServerSetInteractingActor(nullptr);
				InteractingActor = nullptr;
			}
		}
		else if (InteractingActor) {
			Cast<IInteractableObjectInterface>(InteractingActor)->StopHover(this);

			ServerSetInteractingActor(nullptr);
			InteractingActor = nullptr;
		}
	}
}

void APlayerPawn::ServerSetInteractingActor_Implementation(AActor* InteractingObject)
{
	//Serverside Assignment.
	InteractingActor = InteractingObject;
}

float APlayerPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
		HealthComponent->ReduceHealth(DamageAmount);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void APlayerPawn::OnDeath()
{
	if (IsLocallyControlled())
	{
		FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, this->GetActorLocation() + FVector(0, 0, 1200.0f), FVector::OneVector);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Name = FName(*(this->GetActorNameOrLabel() + "_DeathCam"));

		APlayerDeathCamera* PlayerDeathCam = Cast<APlayerDeathCamera>(GetWorld()->SpawnActor(APlayerDeathCamera::StaticClass(), &SpawnTransform, SpawnParams));

		if (PlayerDeathCam)
			PlayerDeathCam->SetCameraPosition(GetActorLocation());

		//Drop all Inventory Items on death
		uint8 ItemCount = InventoryComponent->EquippedRepairObjects.Num();
		for (uint8 i = 0; i < ItemCount; i++)
		{
			InventoryComponent->DropFirstItemFromInventory();
		}
		uint8 WeaponCount = WeaponComponent->GetEquippedWeapons().Num();
		for (uint8 i = 0; i < WeaponCount; i++)
		{
			WeaponComponent->DropFirstWeaponFromInventory();
		}

		//Blueprint Assignable Local Death (Hide HUD, Hide FPS Viewmodel etc.)
		OnPlayerDeathLocal.Broadcast();
	}

	//Deduct a life from the remaining amount in PlayerController_Main
	if (UKismetSystemLibrary::IsServer(GetWorld())) {
		APlayerController_Main* PlayerController = Cast<APlayerController_Main>(GetController());
		if (PlayerController) {
			PlayerController->PlayerRemainingLives--;
			if (PlayerController->PlayerRemainingLives <= 0) {
				PlayerController->PlayerRemainingLives = 0;
				PlayerController->PlayerEndState = EZombieGameWinState::LOST;
			}
		}
	}

	//Blueprint Assignable Replicated Death (Enable Ragdoll, Disable Collision, etc.)
	OnPlayerDeathReplicated.Broadcast();

}

