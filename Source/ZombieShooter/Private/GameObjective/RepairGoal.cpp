
#include "GameObjective/RepairGoal.h"
#include "Kismet/GameplayStatics.h"
#include "General/GameMode_Main.h"

ARepairGoal::ARepairGoal()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh", false);

	RepairProgressText = CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent", false);
	if (Mesh)
		RepairProgressText->SetupAttachment(Mesh);
}

void ARepairGoal::BeginPlay()
{
	Super::BeginPlay();

	if (RepairProgressText)
		TextVisualUpdate();
}

void ARepairGoal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARepairGoal, bRepaired);
}

void ARepairGoal::OnStartInteract_RPC_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::OnStartInteract_RPC_Implementation(InstigatingActor);

	if (bRepaired) {

		//Win Condition when all objectives repaired.
		AGameMode_Main* GameMode = Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()));
		TArray<APawn*> WinningActors = { Cast<APawn>(InstigatingActor) };

		if (GameMode)
			GameMode->EndGameWin(WinningActors);
	}
}

void ARepairGoal::StartHover_RPC_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::StartHover_RPC_Implementation(InstigatingActor);

	if (!bRepaired) {
		StopHover(InstigatingActor);
		return;
	}

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
	}
}

void ARepairGoal::StopHover_RPC_Implementation(AActor* InstigatingActor)
{
	AGenericInteractionActor::StopHover_RPC_Implementation(InstigatingActor);

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

void ARepairGoal::OnRep_RepairStatus()
{
	TextVisualUpdate();
	if (bRepaired)
		StartVehicle();
}

void ARepairGoal::OnRepairedVehicle(APawn* InstigatingActor)
{
	TextVisualUpdate();

	for (uint8 i = 0; i < RequiredRepairObjects.Num(); i++)
	{
		if (!RequiredRepairObjects[i]->bRepaired)
			return;
	}

	bRepaired = true;
	StartVehicle();
}




