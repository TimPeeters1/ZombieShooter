// Project 'Zombie Shooter' -Tim Peeters 2022/23

#include "Interaction/GenericInteractionActor.h"

#include "Characters/Player/PlayerPawn.h"

AGenericInteractionActor::AGenericInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGenericInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGenericInteractionActor::OnInteract_Implementation(AActor* InstigatingActor)
{

}

void AGenericInteractionActor::StartHover_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
	}
}
void AGenericInteractionActor::StopHover_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStopHover();
	}
}


