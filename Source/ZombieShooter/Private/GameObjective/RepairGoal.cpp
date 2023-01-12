
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


void ARepairGoal::OnStartInteract_BP_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::OnStartInteract_BP_Implementation(InstigatingActor);

	if (!bRepaired) return;

	//Win Condition when all objectives repaired.
	AGameMode_Main* GameMode = Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()));
	TArray<APawn*> WinningActors = { Cast<APawn>(InstigatingActor) };

	if (GameMode)
		GameMode->EndGameWin(WinningActors);
}

void ARepairGoal::StartHover_BP_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::StartHover_BP_Implementation(InstigatingActor);

	if (!bRepaired) {
		StopHover(InstigatingActor);
		return;
	}

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
	}
}

void ARepairGoal::StopHover_BP_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::StopHover_BP_Implementation(InstigatingActor);

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
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


