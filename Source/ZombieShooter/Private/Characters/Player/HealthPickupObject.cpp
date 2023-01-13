// Project 'Zombie Shooter' -Tim Peeters 2022/23


#include "Characters/Player/HealthPickupObject.h"

#include "Characters/Player/PlayerPawn.h"

AHealthPickupObject::AHealthPickupObject()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AHealthPickupObject::BeginPlay()
{
	Super::BeginPlay();
	OnHealthSpawned.Broadcast();
}

void AHealthPickupObject::AddHealthToActor_Implementation(AActor* ActorToAdd)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (ActorToAdd)
	{
		APlayerPawn* PlayerPawn = Cast<APlayerPawn>(ActorToAdd);
		if (PlayerPawn)
		{
			if (PlayerPawn->GetHealthComponent()->Health < PlayerPawn->GetHealthComponent()->MaxHealth) {
				PlayerPawn->GetHealthComponent()->AddHealth(HealthToAdd);
				OnHealthPickup.Broadcast();
				SetLifeSpan(0.1f);
			}
		}
	}
}

void AHealthPickupObject::OnStartInteract_BP_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::OnStartInteract_BP_Implementation(InstigatingActor);

	AddHealthToActor(InstigatingActor);
}

void AHealthPickupObject::StartHover_BP_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::StartHover_BP_Implementation(InstigatingActor);
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
	}
}

void AHealthPickupObject::StopHover_BP_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::StopHover_BP_Implementation(InstigatingActor);
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStopHover();
	}
}
