
#include "PlayerPawn.h"

#include "GameMode_Main.h"
#include "PlayerDeathCamera.h"
#include "Components/CapsuleComponent.h"
#include "RepairObjective.h"
#include "GameFramework/GameModeBase.h"


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
	PlayerInputComponent->BindAction("Fire", IE_Released, PlayerWeaponComponent, &UPlayerWeaponComponent::OnFireEnd);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::OnReloadWeapon);
	PlayerInputComponent->BindAction("EquipPrimaryWeapon", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::EquipPrimaryWeapon);
	PlayerInputComponent->BindAction("EquipSecondaryWeapon", IE_Pressed, PlayerWeaponComponent, &UPlayerWeaponComponent::EquipSecondaryWeapon);
}

void APlayerPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerPawn, PlayerWeaponComponent);
}


void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	OnSetPlayerViewMode();

	if(HealthComponent)
		HealthComponent->OnDeath.AddUniqueDynamic(this, &APlayerPawn::OnDeath);
}

void APlayerPawn::OnSetPlayerViewMode()
{
	if (IsLocallyControlled()) {
		GetMesh()->SetVisibility(false, true);
		FP_ArmModel->SetVisibility(true, true);
	}
	else {
		GetMesh()->SetVisibility(true, true);
		FP_ArmModel->SetVisibility(false, true);
	}
}

// Called every frame
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

void APlayerPawn::OnPerformInteraction()
{
	if (!IsLocallyControlled()) return;

	ServerPerformInteraction();

	//This Could be a little overdone, as this is also called from the Server RPC (ServerPerformInteraction). 
	//but as lag would have it, the earlier the actor responds, the better.. right?
	if (InteractingActor && !UKismetSystemLibrary::IsServer(GetWorld()))
		Cast<IInteractableObjectInterface>(InteractingActor)->Execute_OnInteract(InteractingActor);

}

void APlayerPawn::ServerPerformInteraction_Implementation()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (InteractingActor) {
		Cast<IInteractableObjectInterface>(InteractingActor)->Execute_OnInteract(InteractingActor);

		//TEMP Implemenation for Object Pickups.
		if (Cast<ARepairItem>(InteractingActor)) {
			ARepairItem* RepairObject = Cast<ARepairItem>(InteractingActor);
			if (!RepairObjectInventory.Contains(RepairObject)) {
				RepairObjectInventory.Add(RepairObject);
			}
		}
		else if (Cast<ARepairObjective>(InteractingActor)) {
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
					//Replicate InteractingActor to Server
					ServerSetInteractingActor(InteractingActor);

					//Start Local Hover FX
					Cast<IInteractableObjectInterface>(InteractingActor)->Execute_StartHover(InteractingActor);
					OnStartInteraction.Broadcast();
				}
			}
			else if (InteractingActor)
			{

				Cast<IInteractableObjectInterface>(InteractingActor)->Execute_StopHover(InteractingActor);
				OnStopInteraction.Broadcast();

				ServerSetInteractingActor(nullptr);
				InteractingActor = nullptr;
			}
		}
		else if (InteractingActor) {

			Cast<IInteractableObjectInterface>(InteractingActor)->Execute_StopHover(InteractingActor);
			OnStopInteraction.Broadcast();

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

		if(PlayerDeathCam)
			PlayerDeathCam->SetCameraPosition(GetActorLocation());

		//Blueprint Assignable Local Death (Hide HUD, Hide FPS Viewmodel etc.)
		OnPlayerDeathLocal.Broadcast();
	}

	//Blueprint Assignable Replicated Death (Enable Ragdoll, Disable Collision, etc.)
	OnPlayerDeathReplicated.Broadcast();

}

