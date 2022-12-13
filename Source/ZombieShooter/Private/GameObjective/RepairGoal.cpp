
#include "GameObjective/RepairGoal.h"
#include "Kismet/GameplayStatics.h"
#include "General/GameMode_Main.h"

ARepairGoal::ARepairGoal()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ObjectMeshComponent", false);
	if (ObjectMesh)
		ObjectMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	RepairProgressText = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent", false);
	if (RepairProgressText)
		RepairProgressText->SetupAttachment(ObjectMesh);
}

void ARepairGoal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARepairGoal, RepairObjectAmount);
	DOREPLIFETIME(ARepairGoal, CurrentRepairedAmount);
}

void ARepairGoal::BeginPlay()
{
	Super::BeginPlay();

	if(RepairProgressText)
		TextVisualUpdate();
}

void ARepairGoal::OnInteract_Implementation()
{
}

void ARepairGoal::StartHover_Implementation()
{

}

void ARepairGoal::StopHover_Implementation()
{
}

void ARepairGoal::OnRep_RepairAmount()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Repair Callback!")));

	TextVisualUpdate();
}

void ARepairGoal::OnObjectiveRepaired()
{
	AGameMode_Main* GameMode = Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
		GameMode->EndGame(EZombieGameEndGameState::WON);
}



void ARepairGoal::TextVisualUpdate()
{
	if (!RepairProgressText) return;

	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(CurrentRepairedAmount));
	Args.Add(FStringFormatArg(RepairObjectAmount));
	FString RepairCompletion = FString::Format(TEXT("Repaired: {0}/{1}"), Args);
	RepairProgressText->SetText(FText::FromString(RepairCompletion));
}

void ARepairGoal::AddRepairObject(ARepairObject* RepairObject)
{
	if (!CollectedRepairObjects.Contains(RepairObject) && CurrentRepairedAmount < RepairObjectAmount) {
		CollectedRepairObjects.Add(RepairObject);
		CurrentRepairedAmount++;

		TextVisualUpdate();

		if (CurrentRepairedAmount == RepairObjectAmount)
			OnObjectiveRepaired();
	}
}
