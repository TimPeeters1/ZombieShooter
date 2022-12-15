
#include "GameObjective/RepairObject.h"
#include "Characters/Player/PlayerPawn.h"

ARepairObject::ARepairObject()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMeshComponent", false);
	SetRootComponent(ObjectMesh);
}

void ARepairObject::OnInteract_Implementation(AActor* InteractionInstigator)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InteractionInstigator);
	if (PlayerPawn) {
		PlayerPawn->GetInventoryComponent()->AddObjectToInventory(this);
		InteractRPC();
	}
}

void ARepairObject::StartHover_Implementation()
{
}

void ARepairObject::StopHover_Implementation()
{
}

void ARepairObject::InteractRPC_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}




