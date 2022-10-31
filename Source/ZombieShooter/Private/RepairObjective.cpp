
#include "RepairObjective.h"

ARepairObjective::ARepairObjective()
{
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ObjectMeshComponent", false);
	if (ObjectMesh)
		ObjectMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	RepairProgressText = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent", false);

}

void ARepairObjective::OnInteract_Implementation()
{
}

void ARepairObjective::StartHover_Implementation()
{

}

void ARepairObjective::StopHover_Implementation()
{
}

void ARepairObjective::AddRepairItem(ARepairItem* RepairObject)
{
	if (!CollectedRepairItems.Contains(RepairObject)) {
		CollectedRepairItems.Add(RepairObject);
	}
}
