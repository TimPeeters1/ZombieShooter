
#include "GameObjective/RepairGoal.h"
#include "Kismet/GameplayStatics.h"
#include "General/GameMode_Main.h"

ARepairGoal::ARepairGoal()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ObjectMeshComponent", false);

	RepairProgressText = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent", false);
	if (RepairProgressText)
		RepairProgressText->SetupAttachment(ObjectMesh);
}

void ARepairGoal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ARepairGoal::BeginPlay()
{
	Super::BeginPlay();

	if (RepairProgressText)
		TextVisualUpdate();
}


void ARepairGoal::OnRep_RepairAmount()
{
	TextVisualUpdate();
}


void ARepairGoal::OnStartInteract_Implementation(AActor* InteractionInstigator)
{
	if (!bRepaired) return;

	//Win Condition when all objectives repaired.
	AGameMode_Main* GameMode = Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()));
	TArray<APawn*> WinningActors = { Cast<APawn>(InteractionInstigator) };

	if (GameMode)
		GameMode->EndGameWin(WinningActors);
}

void ARepairGoal::StartHover_Implementation(AActor* InteractionInstigator)
{
	if (!bRepaired) {
		StopHover(InteractionInstigator);
		return;
	}

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InteractionInstigator);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
	}
}

void ARepairGoal::StopHover_Implementation(AActor* InteractionInstigator)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InteractionInstigator);
	if (PlayerPawn) {
		PlayerPawn->OnStopHover();
	}
}

void ARepairGoal::TextVisualUpdate()
{
	if (!RepairProgressText) return;

	uint8 requiredRepairObjects = 0;
	for (uint8 i = 0; i < RequiredRepairObjects.Num(); i++)
	{
		if (RequiredRepairObjects[i]->bRepaired)
			requiredRepairObjects++;
	}

	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(requiredRepairObjects));
	Args.Add(FStringFormatArg(RequiredRepairObjects.Num()));
	FString RepairCompletion = FString::Format(TEXT("Repaired: {0}/{1}"), Args);
	RepairProgressText->SetText(FText::FromString(RepairCompletion));
}

void ARepairGoal::OnRepairedObject_Implementation(APawn* InstigatingActor)
{
	TextVisualUpdate();

	for (uint8 i = 0; i < RequiredRepairObjects.Num(); i++)
	{
		if (!RequiredRepairObjects[i]->bRepaired)
			return;
	}

	bRepaired = true;
}


