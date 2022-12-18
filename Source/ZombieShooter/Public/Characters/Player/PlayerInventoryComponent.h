// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "NavigationSystem.h"

#include "GameObjective/RepairObject.h"

#include "PlayerInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESHOOTER_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInventoryComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
	FOnInventoryUpdate InventoryUpdate;
public:	
	UPROPERTY(ReplicatedUsing = OnInventoryChanged, Category = "Inventory|Equipped RepairObject", EditInstanceOnly, BlueprintReadOnly)
		TArray<ARepairObject*> EquippedRepairObjects;

	UFUNCTION(Server, Reliable)
	void AddObjectToInventory(ARepairObject* ObjectToAdd);
	void AddObjectToInventory_Implementation(ARepairObject* ObjectToAdd);

	UFUNCTION(Server, Reliable)
	void RemoveObjectFromInventory(ARepairObject* ObjectToRemove);
	void RemoveObjectFromInventory_Implementation(ARepairObject* ObjectToRemove);

	UFUNCTION()
	void OnInventoryChanged();

	UFUNCTION(Server, Reliable)
	void DropFirstItemFromInventory();
	void DropFirstItemFromInventory_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
