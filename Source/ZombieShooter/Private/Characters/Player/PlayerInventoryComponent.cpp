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
}

void UPlayerInventoryComponent::RemoveObjectFromInventory_Implementation(ARepairObject* ObjectToRemove)
{
	if (EquippedRepairObjects.Contains(ObjectToRemove))
		EquippedRepairObjects.Remove(ObjectToRemove);
}

void UPlayerInventoryComponent::OnInventoryChanged()
{
	InventoryUpdate.Broadcast();
}

void UPlayerInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerInventoryComponent, EquippedRepairObjects);
}


