// Project 'Zombie Shooter' -Tim Peeters 2022/23


#include "Characters/Player/HealthPickupPoint.h"

#include <Kismet/KismetSystemLibrary.h>

AHealthPickupPoint::AHealthPickupPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AHealthPickupPoint::BeginPlay()
{
	Super::BeginPlay();

	SpawnHealthPickup();
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

void AHealthPickupPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHealthPickupPoint, CurrentHealthObject);
}

void AHealthPickupPoint::SpawnHealthPickup()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform = FTransform(GetActorRotation(), GetActorLocation() + FVector(0, 0, 60.0f), FVector::OneVector);

	CurrentHealthObject = GetWorld()->SpawnActor<AHealthPickupObject>(HealthPickupActor, SpawnTransform, SpawnParams);

	CurrentHealthObject->OnHealthPickup.AddDynamic(this, &AHealthPickupPoint::OnPickup);

	GetWorld()->GetTimerManager().PauseTimer(SpawnTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

void AHealthPickupPoint::OnPickup()
{
	if (CurrentHealthObject) {
		CurrentHealthObject->OnHealthPickup.RemoveDynamic(this, &AHealthPickupPoint::OnPickup);
		CurrentHealthObject = nullptr;

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AHealthPickupPoint::SpawnHealthPickup, GenerateSpawnDelay(), false, -1.0f);
	}
}


