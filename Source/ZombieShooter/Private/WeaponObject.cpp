
#include "WeaponObject.h"

// Sets default values
AWeaponObject::AWeaponObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponModel = CreateDefaultSubobject<UStaticMeshComponent>("WeaponModel");
	RootComponent = WeaponModel;
}


void AWeaponObject::OnConstruction(const FTransform& Transform)
{
	if (WeaponData != nullptr) {
		WeaponModel->SetStaticMesh(WeaponData->WeaponModel);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, WeaponData->WeaponModel->GetName());
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

