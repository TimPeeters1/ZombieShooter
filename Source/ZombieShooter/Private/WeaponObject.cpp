
#include "WeaponObject.h"


// Sets default values
AWeaponObject::AWeaponObject()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);

	WeaponModel = CreateDefaultSubobject<UStaticMeshComponent>("WeaponModel");
	RootComponent = WeaponModel;
}

void AWeaponObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponObject, WeaponData);
	DOREPLIFETIME(AWeaponObject, MagazineSize);
	DOREPLIFETIME(AWeaponObject, MaxAmmo);
	DOREPLIFETIME(AWeaponObject, CurrentAmmo);
}
void AWeaponObject::OnConstruction(const FTransform& Transform)
{
	if (!WeaponData) return;

	WeaponModel->SetStaticMesh(WeaponData->WeaponModel);

	MagazineSize = WeaponData->Weapon_Default_MagazineSize;
	MaxAmmo = WeaponData->Weapon_Default_MaxAmmo;
	//remove this line for customizable ammo variable for spawned
	CurrentAmmo = MagazineSize;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Start Ammo: %d"), CurrentAmmo));
}

// Called when the game starts or when spawned
void AWeaponObject::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponObject::OnRep_WeaponData()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Weapon Data Set!")));
}

void AWeaponObject::OnRep_MagazineSizeUpdate()
{
}

void AWeaponObject::OnRep_MaxAmmoUpdate()
{
}

void AWeaponObject::OnRep_AmmoUpdate()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Updated Ammo: %d"), CurrentAmmo));
}

// Called every frame
void AWeaponObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




