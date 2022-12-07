
#include "RepairObjective.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode_Main.h"

ARepairObjective::ARepairObjective()
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

void ARepairObjective::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARepairObjective, RepairItemAmount);
	DOREPLIFETIME(ARepairObjective, CurrentRepairedAmount);
}

void ARepairObjective::BeginPlay()
{
	Super::BeginPlay();

	if(RepairProgressText)
		TextVisualUpdate();
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

void ARepairObjective::OnRep_RepairAmount()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Repair Callback!")));

	TextVisualUpdate();
}

void ARepairObjective::OnObjectiveRepaired()
{
	AGameMode_Main* GameMode = Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
		GameMode->EndGame(EZombieGameEndGameState::WON);
}



void ARepairObjective::TextVisualUpdate()
{
	if (!RepairProgressText) return;

	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(CurrentRepairedAmount));
	Args.Add(FStringFormatArg(RepairItemAmount));
	FString RepairCompletion = FString::Format(TEXT("Repaired: {0}/{1}"), Args);
	RepairProgressText->SetText(FText::FromString(RepairCompletion));
}

void ARepairObjective::AddRepairItem(ARepairItem* RepairObject)
{
	if (!CollectedRepairItems.Contains(RepairObject) && CurrentRepairedAmount < RepairItemAmount) {
		CollectedRepairItems.Add(RepairObject);
		CurrentRepairedAmount++;

		TextVisualUpdate();

		if (CurrentRepairedAmount == RepairItemAmount)
			OnObjectiveRepaired();
	}
}
