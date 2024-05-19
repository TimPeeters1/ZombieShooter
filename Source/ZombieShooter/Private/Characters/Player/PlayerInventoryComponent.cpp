// Project 'Zombie Shooter' -Tim Peeters 2022/23

#include "Characters/Player/PlayerInventoryComponent.h"
#include "Net/UnrealNetwork.h"


UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPlayerInventoryComponent::AddObjectToInventory_Implementation(ARepairObject* ObjectToAdd)
{
	if (!EquippedRepairObjects.Contains(ObjectToAdd))
		EquippedRepairObjects.Add(ObjectToAdd);

	//Serverside Update
	InventoryUpdate.Broadcast();
}

void UPlayerInventoryComponent::RemoveObjectFromInventory_Implementation(ARepairObject* ObjectToRemove)
{
	if (EquippedRepairObjects.Contains(ObjectToRemove))
		EquippedRepairObjects.Remove(ObjectToRemove);

	//Serverside Update
	InventoryUpdate.Broadcast();
}

void UPlayerInventoryComponent::OnInventoryChanged()
{
	//Clientside Update
	InventoryUpdate.Broadcast();
}

void UPlayerInventoryComponent::DropFirstItemFromInventory_Implementation()
{
	if (EquippedRepairObjects.IsEmpty()) return;

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavigationSystem) return;

	FNavLocation DropLocation;
	if (NavigationSystem->GetRandomPointInNavigableRadius(GetOwner()->GetActorLocation(), 300.0f, DropLocation)) {

		//DrawDebugPoint(GetWorld(), DropLocation, 5.0f, FColor::Red, false, 5.0f);

		FVector Origin;
		FVector Extents;
		EquippedRepairObjects[0]->GetActorBounds(true, Origin, Extents);
		DropLocation.Location += FVector(0, 0, Extents.Z);
		EquippedRepairObjects[0]->SetActorLocation(DropLocation);
		EquippedRepairObjects[0]->Dequip();
		RemoveObjectFromInventory(EquippedRepairObjects[0]);
	}
}

void UPlayerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerInventoryComponent, EquippedRepairObjects);
}


