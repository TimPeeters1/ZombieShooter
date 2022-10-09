
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

	DOREPLIFETIME(AWeaponObject, MaxAmmo);
	DOREPLIFETIME(AWeaponObject, CurrentAmmo);
}
void AWeaponObject::OnConstruction(const FTransform& Transform)
{
	if (WeaponData != nullptr) {
		WeaponModel->SetStaticMesh(WeaponData->WeaponModel);
	}
}

// Called when the game starts or when spawned
void AWeaponObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


uint8 AWeaponObject::SetAmmo(uint8 newAmount)
{
	CurrentAmmo = newAmount;
	return CurrentAmmo;
}

uint8 AWeaponObject::SetMaxAmmo(uint8 newMaxAmount)
{
	MaxAmmo = newMaxAmount;
	return MaxAmmo;
}



