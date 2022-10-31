
#include "RepairItem.h"

ARepairItem::ARepairItem()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMeshComponent", false);
	SetRootComponent(ObjectMesh);

	if (ObjectMesh)
		ObjectMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ARepairItem::OnInteract_Implementation()
{
	MC_ServerInteract();
}

void ARepairItem::StartHover_Implementation()
{
}

void ARepairItem::StopHover_Implementation()
{
}

void ARepairItem::MC_ServerInteract_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}




