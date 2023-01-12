// Project 'Zombie Shooter' -Tim Peeters 2022/23


#include "Characters/Player/HealthPickupPoint.h"

#include <Kismet/KismetSystemLibrary.h>

AHealthPickupPoint::AHealthPickupPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AHealthPickupPoint::BeginPlay()
{
	Super::BeginPlay();
	SpawnHealthPickup();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AHealthPickupPoint::SpawnHealthPickup, GenerateSpawnDelay(), false);
}

void AHealthPickupPoint::Tick(float Delta)
{
	Super::Tick(Delta);
	if (CurrentHealthObject) {
		CurrentHealthObject->AddActorWorldRotation((FRotator(0, MeshRotationSpeed, 0) * Delta));
	}
}

float AHealthPickupPoint::GenerateSpawnDelay()
{
	return FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
}

void AHealthPickupPoint::SpawnHealthPickup()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform = FTransform(GetActorRotation(), GetActorLocation() + FVector(0, 0, 60.0f), FVector::OneVector);

	CurrentHealthObject = GetWorld()->SpawnActor<AHealthPickupObject>(HealthPickupActor, SpawnTransform, SpawnParams);
}

void AHealthPickupPoint::OnPickup()
{
	if (CurrentHealthObject) {
		CurrentHealthObject = nullptr;

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AHealthPickupPoint::SpawnHealthPickup, GenerateSpawnDelay(), false);
	}
}


