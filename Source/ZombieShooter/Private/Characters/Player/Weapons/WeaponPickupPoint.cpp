// Project 'Zombie Shooter' -Tim Peeters 2022/23


#include "Characters/Player/Weapons/WeaponPickupPoint.h"


// Sets default values
AWeaponPickupPoint::AWeaponPickupPoint()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AWeaponPickupPoint::BeginPlay()
{
	SpawnWeaponPickup();
}

void AWeaponPickupPoint::SpawnWeaponPickup_Implementation()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	AGameMode_Main* GameMode = Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode && !SpawnableWeaponTypes.IsEmpty()) {


		
		FTransform SpawnTransform = FTransform(GetActorRotation(), GetActorLocation(), FVector::OneVector);
		AWeaponObject* NewWeaponObject = (AWeaponObject*)GetWorld()->SpawnActorDeferred<AWeaponObject>(AWeaponObject::StaticClass(),
			SpawnTransform);

		NewWeaponObject->SetReplicates(true);

		uint8 WeaponObjectIndex = FMath::RandRange(0, (SpawnableWeaponTypes.Num() - 1));
		NewWeaponObject->WeaponData = SpawnableWeaponTypes[WeaponObjectIndex];

		UGameplayStatics::FinishSpawningActor(NewWeaponObject, NewWeaponObject->GetTransform());

		NewWeaponObject->OnWeaponPikcup.AddDynamic(this, &AWeaponPickupPoint::OnPickup);
		CurrentWeaponObject = NewWeaponObject;
	}
}

void AWeaponPickupPoint::OnPickup()
{
	if (CurrentWeaponObject) {
		CurrentWeaponObject->OnWeaponPikcup.RemoveDynamic(this, &AWeaponPickupPoint::OnPickup);
		CurrentWeaponObject = nullptr;
	}
}






