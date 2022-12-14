// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "RepairGoal.h"

#include "RepairGoal_Trigger.generated.h"

UCLASS()
class ZOMBIESHOOTER_API ARepairGoal_Trigger : public ATriggerBox, public IInteractableObjectInterface
{
	GENERATED_BODY()
public:
	ARepairGoal_Trigger();
protected:
	ARepairGoal* RepairGoalParent;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract();
	virtual void OnInteract_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartHover();
	virtual void StartHover_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StopHover();
	virtual void StopHover_Implementation() override;

public:
	virtual void BeginPlay() override;
};
