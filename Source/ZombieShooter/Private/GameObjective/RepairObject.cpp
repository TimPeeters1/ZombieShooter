
#include "GameObjective/RepairObject.h"
#include "Characters/Player/PlayerPawn.h"

ARepairObject::ARepairObject()
{
	SetReplicates(true);
	SetReplicateMovement(true);
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMeshComponent", false);
	SetRootComponent(ObjectMesh);
}

void ARepairObject::OnInteract_Implementation(AActor* InteractionInstigator)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InteractionInstigator);
	if (PlayerPawn) {
		PlayerPawn->GetInventoryComponent()->AddObjectToInventory(this);
		OnEquip();
	}
}

void ARepairObject::StartHover_Implementation()
{
}

void ARepairObject::StopHover_Implementation()
{
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




