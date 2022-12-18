// Project 'Zombie Shooter' -Tim Peeters 2022/23

#include "GameObjective/RepairGoal_Trigger.h"

#include "Characters/Player/PlayerPawn.h"
#include "GameObjective/RepairGoal.h"

ARepairGoal_Trigger::ARepairGoal_Trigger()
{
	SetReplicates(true);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision", false);
	BoxCollision->SetBoxExtent(FVector::OneVector * 50.0f);
	SetRootComponent(BoxCollision);

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMeshComponent", false);
	ObjectMesh->SetupAttachment(RootComponent);
	ObjectMesh->SetHiddenInGame(false, true);

	RepairObjectText = CreateDefaultSubobject<UTextRenderComponent>("RepairObjectText", false);
	RepairObjectText->SetupAttachment(ObjectMesh);
}

void ARepairGoal_Trigger::BeginPlay()
{
	Super::BeginPlay();

	ARepairGoal* RepairGoalParent = Cast<ARepairGoal>(GetAttachParentActor());
	if (RepairGoalParent) {
		RepairGoalParent->RequiredRepairObjects.Add(this);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("RepairGoal_Trigger No Parent Found!"));
	}
}

void ARepairGoal_Trigger::OnInteract_Implementation(AActor* InteractionInstigator)
{
	if (bRepaired) return;

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InteractionInstigator);
	if (PlayerPawn) { 
		ARepairObject* DesiredRepairObject = nullptr;
		for (uint8 i = 0; i < PlayerPawn->GetInventoryComponent()->EquippedRepairObjects.Num(); i++)
		{
			if (PlayerPawn->GetInventoryComponent()->EquippedRepairObjects[i]->ObjectData->ObjectType == ObjectData->ObjectType)
				DesiredRepairObject = PlayerPawn->GetInventoryComponent()->EquippedRepairObjects[i];
		}

		if (DesiredRepairObject != nullptr) {
			PlayerPawn->GetInventoryComponent()->RemoveObjectFromInventory(DesiredRepairObject);

			if(Cast<APawn>(InteractionInstigator))
				OnRepairedObject(Cast<APawn>(InteractionInstigator));
		}
	}
}

void ARepairGoal_Trigger::StartHover_Implementation()
{
}
 
void ARepairGoal_Trigger::StopHover_Implementation()
{
}

void ARepairGoal_Trigger::OnRepairedObject_Implementation(APawn* InstigatingActor)
{
	if (!bRepaired) {
		OnRepaired.Broadcast();
		bRepaired = true;

		ARepairGoal* RepairGoalParent = Cast<ARepairGoal>(GetAttachParentActor());
		if (RepairGoalParent) {
			RepairGoalParent->OnRepairedObject(InstigatingActor);
		}
	}
}

