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

void AGenericInteractionActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGenericInteractionActor::OnStartInteract_RPC_Implementation(AActor* InstigatingActor)
{
	if (bHolddown) {
		bIsHolding = true;
		OnHolddown_Event.Broadcast();

		HolddownDelegate = FTimerDelegate::CreateUObject(this, &AGenericInteractionActor::OnCompleteHolddown_RPC, InstigatingActor);

		GetWorld()->GetTimerManager().SetTimer(HolddownTimer, HolddownDelegate, HoldTime, false);
	}
	else {
		OnInteraction_Event.Broadcast();
	}
	OnStartInteract_BP(InstigatingActor);
}

void AGenericInteractionActor::OnStopInteract_RPC_Implementation(AActor* InstigatingActor)
{
	if (bHolddown) {
		bIsHolding = false;
		GetWorld()->GetTimerManager().ClearTimer(HolddownTimer);
	}
	OnStopInteract_BP(InstigatingActor);
	
}

void AGenericInteractionActor::StartHover_RPC_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
		OnStartHover_Event.Broadcast();
	}
	StartHover_BP(InstigatingActor);
}

void AGenericInteractionActor::StopHover_RPC_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStopHover();
		OnStopHover_Event.Broadcast();
	}
	StopHover_BP(InstigatingActor);
}

void AGenericInteractionActor::OnCompleteHolddown_RPC_Implementation(AActor* InstigatingActor)
{
	if (bIsHolding) {
		OnInteraction_Event.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(HolddownTimer);
	}
	OnCompleteHolddown_BP(InstigatingActor);
}



