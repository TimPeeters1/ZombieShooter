
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
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("Repair Callback!")));

	TextVisualUpdate();
}

void ARepairGoal::OnObjectiveRepaired()
{
	AGameMode_Main* GameMode = Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
		GameMode->EndGame();
}

void ARepairGoal::OnInteract_Implementation(AActor* InteractionInstigator)
{


}

void ARepairGoal::TextVisualUpdate()
{
	if (!RepairProgressText) return;

	/*
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(CurrentRepairedAmount));
	Args.Add(FStringFormatArg(RepairObjectAmount));
	FString RepairCompletion = FString::Format(TEXT("Repaired: {0}/{1}"), Args);
	RepairProgressText->SetText(FText::FromString(RepairCompletion));
	*/
}

void ARepairGoal::OnRepairedObject()
{
	for (uint8 i = 0; i < RequiredRepairObjects.Num(); i++)
	{
		if (!RequiredRepairObjects[i]->bRepaired)
			return;
	}

	OnObjectiveRepaired();
}


