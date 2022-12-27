
#include "Characters/Player/Weapons/WeaponObject.h"
#include "Characters/Player/PlayerPawn.h"

// Sets default values
AWeaponObject::AWeaponObject()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponModel = CreateDefaultSubobject<UStaticMeshComponent>("WeaponModel");
	RootComponent = WeaponModel;
	WeaponModel->SetIsReplicated(true);
}

void AWeaponObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponObject, WeaponData);

	DOREPLIFETIME(AWeaponObject, MagazineSize);
	DOREPLIFETIME(AWeaponObject, MaxInventoryAmmo);

	DOREPLIFETIME(AWeaponObject, InventoryAmmo);
	DOREPLIFETIME(AWeaponObject, CurrentAmmo);
}

void AWeaponObject::OnConstruction(const FTransform& Transform)
{
	if (!WeaponData) return;

	WeaponModel->SetStaticMesh(WeaponData->WeaponMesh);

	MagazineSize = WeaponData->Weapon_Default_MagazineSize;
	MaxInventoryAmmo = WeaponData->Weapon_Default_MaxAmmo;

	//Init Ammo Variables
	InventoryAmmo = MaxInventoryAmmo - MagazineSize;
	CurrentAmmo = MagazineSize;
}

// Called when the game starts or when spawned
void AWeaponObject::BeginPlay()
{
	Super::BeginPlay();
	LocalCurrentAmmo = CurrentAmmo;
	OnWeaponSpawned.Broadcast();
}

void AWeaponObject::OnRep_WeaponData()
{
	WeaponModel->SetStaticMesh(WeaponData->WeaponMesh);
}

void AWeaponObject::OnRep_MagazineSizeUpdate()
{
}

void AWeaponObject::OnRep_MaxInventoryAmmoUpdate()
{
}

void AWeaponObject::OnRep_InventoryAmmoUpdate()
{
}

void AWeaponObject::OnRep_CurrentAmmoUpdate()
{
	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Current Ammo: %d"), CurrentAmmo));
	
	LocalCurrentAmmo = CurrentAmmo;
}

void AWeaponObject::Equip_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	WeaponModel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponObject::Dequip_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	WeaponModel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AWeaponObject::OnInteract_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->GetWeaponComponent()->OnPickupWeapon(this);
	}
}

void AWeaponObject::StartHover_Implementation(AActor* InteractionInstigator)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InteractionInstigator);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
	}
}

void AWeaponObject::StopHover_Implementation(AActor* InteractionInstigator)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InteractionInstigator);
	if (PlayerPawn) {
		PlayerPawn->OnStopHover();
	}
}






