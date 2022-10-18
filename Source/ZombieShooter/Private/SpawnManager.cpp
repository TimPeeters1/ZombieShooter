


#include "SpawnManager.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	if (UKismetSystemLibrary::IsServer(GetWorld())) {
		GetWorldTimerManager().SetTimer(PlayerSweepTimer, this, &ASpawnManager::AreaSweep, AreaSweepTimer, true, 5.0f);
	}
}

void ASpawnManager::PopulateGame()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (!ActiveAreaSet.IsEmpty())
		for (uint8 i = 0; i < ActiveAreaSet.Num(); i++)
		{
			uint8 DesiredZombieCount = ActiveAreaSet[i]->DesiredEnemyCount;
			for (uint8 j = 0; j < DesiredZombieCount; j++)
			{
				ActiveAreaSet[i]->SpawnEnemy(SpawnableEnemies[0]);
			}
		}
}

void ASpawnManager::InitWave()
{
}

void ASpawnManager::AreaSweep()
{
	AGameMode_Main* GameMode = (AGameMode_Main*)UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode->PlayerCharacters.IsEmpty()) return;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	TArray<AActor*> ignoredActors;
	ignoredActors.Add(GetOwner());

	TArray<ASpawnArea*> overlappedAreas;

	for (uint8 i = 0; i < GameMode->PlayerCharacters.Num(); i++)
	{
		if (!GameMode->PlayerCharacters[i]->GetPawn()) return;

		TArray<AActor*> overlappedActors;

		if (bDrawDebug)
			DrawDebugSphere(GetWorld(), GameMode->PlayerCharacters[i]->GetPawn()->GetActorLocation(), AreaSweepRange, 26, FColor::Purple, false, 2, 0, 5);

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GameMode->PlayerCharacters[i]->GetPawn()->GetActorLocation(), AreaSweepRange,
			traceObjectTypes, ASpawnArea::StaticClass(),
			ignoredActors, overlappedActors);

		for (uint8 j = 0; j < overlappedActors.Num(); j++)
		{
			overlappedAreas.Add((ASpawnArea*)overlappedActors[j]);
		}
	}

	if (!overlappedAreas.IsEmpty()) {
		for (uint8 g = 0; g < ActiveAreaSet.Num(); g++)
		{
			if (!overlappedAreas.Contains(ActiveAreaSet[g])) {
				ActiveAreaSet[g]->SetAreaStatus(false);
			}
		}
		ActiveAreaSet.Empty();

		for (uint8 f = 0; f < overlappedAreas.Num(); f++)
		{
			if (!ActiveAreaSet.Contains(overlappedAreas[f])) {
				ActiveAreaSet.Add(overlappedAreas[f]);
				overlappedAreas[f]->SetAreaStatus(true);
			}
		}
	}

	for (uint8 h = 0; h < ActiveAreaSet.Num(); h++)
	{
		if (ActiveAreaSet[h]->ContainsPlayers()) {
			ActiveAreaSet[h]->SetAreaStatus(false);
			ActiveAreaSet.RemoveAt(h);
		}
	}

}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

