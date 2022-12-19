
#include "GameObjective/RepairObject.h"
#include "Characters/Player/PlayerPawn.h"
#include "Characters/Player/PlayerController_Main.h"

ARepairObject::ARepairObject()
{
	SetReplicates(true);
	SetReplicateMovement(true);
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMeshComponent", false);
	SetRootComponent(ObjectMesh);
}

void ARepairObject::OnInteract_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->GetInventoryComponent()->AddObjectToInventory(this);
		OnEquip();
	}
}

void ARepairObject::StartHover_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
	}
}

void ARepairObject::StopHover_Implementation(AActor* InstigatingActor)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStopHover();
	}
}

void ARepairObject::OnEquip_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ARepairObject::OnDequip_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}




