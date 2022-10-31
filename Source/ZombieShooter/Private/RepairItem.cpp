


#include "RepairItem.h"

ARepairItem::ARepairItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMeshComponent", false);
	SetRootComponent(ObjectMesh);

	if (ObjectMesh)
		ObjectMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ARepairItem::OnInteract_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ARepairItem::StartHover_Implementation()
{
}

void ARepairItem::StopHover_Implementation()
{
}




