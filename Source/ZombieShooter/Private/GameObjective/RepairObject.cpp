
#include "GameObjective/RepairObject.h"
#include "Characters/Player/PlayerPawn.h"
#include "Characters/Player/PlayerController_Main.h"

ARepairObject::ARepairObject()
{
	bReplicates = true;
	SetReplicateMovement(true);
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMeshComponent", false);
	SetRootComponent(ObjectMesh);
}

void ARepairObject::OnStartInteract_RPC_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::OnStartInteract_RPC_Implementation(InstigatingActor);

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->GetInventoryComponent()->AddObjectToInventory(this);
		Equip();
	}
}

void ARepairObject::StartHover_RPC_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::StartHover_RPC_Implementation(InstigatingActor);

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
	}
}

void ARepairObject::StopHover_RPC_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::StopHover_RPC_Implementation(InstigatingActor);

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStopHover();
	}
}

void ARepairObject::Equip_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ARepairObject::Dequip_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}




