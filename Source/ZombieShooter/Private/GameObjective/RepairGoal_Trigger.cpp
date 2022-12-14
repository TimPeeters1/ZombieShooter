// Project 'Zombie Shooter' -Tim Peeters 2022/23

#include "GameObjective/RepairGoal_Trigger.h"


ARepairGoal_Trigger::ARepairGoal_Trigger()
{

}

void ARepairGoal_Trigger::OnInteract_Implementation(AActor* InteractionInstigator)
{
}

void ARepairGoal_Trigger::StartHover_Implementation()
{
}
 
void ARepairGoal_Trigger::StopHover_Implementation()
{
}

void ARepairGoal_Trigger::BeginPlay()
{
	
	RepairGoalParent = Cast<ARepairGoal>(GetParentActor());
	if(RepairGoalParent)
		UE_LOG(LogTemp, Error, TEXT("RepairGoal_Trigger No Parent Found!"));
}
