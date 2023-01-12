// Project 'Zombie Shooter' -Tim Peeters 2022/23

#include "Interaction/GenericInteractionActor.h"

#include "Characters/Player/PlayerPawn.h"

AGenericInteractionActor::AGenericInteractionActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AGenericInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGenericInteractionActor::OnStartInteract(AActor* InstigatingActor)
{
	IInteractableObjectInterface::OnStartInteract(InstigatingActor);
	OnStartInteract_BP(InstigatingActor);
}

void AGenericInteractionActor::OnStopInteract(AActor* InstigatingActor)
{
	IInteractableObjectInterface::OnStopInteract(InstigatingActor);
	OnStopInteract_BP(InstigatingActor);
}

void AGenericInteractionActor::StartHover(AActor* InstigatingActor)
{
	IInteractableObjectInterface::StartHover(InstigatingActor);
	StartHover_BP(InstigatingActor);
}

void AGenericInteractionActor::StopHover(AActor* InstigatingActor)
{
	IInteractableObjectInterface::StopHover(InstigatingActor);
	StopHover_BP(InstigatingActor);
}

void AGenericInteractionActor::OnStartInteract_BP_Implementation(AActor* InstigatingActor)
{
	if (bHolddown) {
		bIsHolding = true;
		OnHolddown_Event.Broadcast();
		GetWorld()->GetTimerManager().SetTimer(HolddownTimer, this, &AGenericInteractionActor::OnCompleteHolddown, HoldTime, false);
	}
	else {
		OnInteraction_Event.Broadcast();
	}
}

void AGenericInteractionActor::OnStopInteract_BP_Implementation(AActor* InstigatingActor)
{
	if (bHolddown) {
		bIsHolding = false;
		GetWorld()->GetTimerManager().ClearTimer(HolddownTimer);
	}
}

void AGenericInteractionActor::OnCompleteHolddown_Implementation()
{
	if (bIsHolding) {
		OnInteraction_Event.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(HolddownTimer);
	}
}

void AGenericInteractionActor::StartHover_BP_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
		OnStartHover_Event.Broadcast();
	}
}
void AGenericInteractionActor::StopHover_BP_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStopHover();
		OnStopHover_Event.Broadcast();
	}
}


