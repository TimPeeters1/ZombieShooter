#include "Spawning/SpawnManager.h"

void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = (UGameInstance_Main*)UGameplayStatics::GetGameInstance(GetWorld());

	if (GameInstance)
		GameInstance->SpawnManager = this;
}

void ASpawnManager::StartSpawningRoutines(float StartDelay)
{
	if (UKismetSystemLibrary::IsServer(GetWorld())) {
		GetWorldTimerManager().SetTimer(ActiveAreaSweepTimer, this, &ASpawnManager::ActiveAreaSweep, ActiveAreaSweepInterval, true, StartDelay);

		GetWorldTimerManager().SetTimer(PopCheckTimer, this, &ASpawnManager::CheckPopulation, PopulationCheckInterval, true, StartDelay + 0.1f);
	}

}

void ASpawnManager::CheckPopulation()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;
	if (GameInstance->GetZombieGameState() != EZombieGameState::INGAME) return;

	if (ActiveAreaSet.IsEmpty()) return;

	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Current Population: %d"), Current_AI_Population));


	if (Current_AI_Population < Max_AI_Population) {
		uint8 AI_Pop_Budget = Max_AI_Population - Current_AI_Population;

		for (uint8 i = 0; i < AI_Pop_Budget; i++)
		{
			uint8 RandomArea = FMath::RandRange(0, ActiveAreaSet.Num() - 1);
			if (ActiveAreaSet[RandomArea]->SpawnEnemy(SpawnableEnemies[0])) {
				Current_AI_Population++;
			}
		}

		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Populating With %d New Enemies!"), AI_Pop_Budget));
	}
}


void ASpawnManager::ActiveAreaSweep()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;
	if (GameInstance->GetZombieGameState() != EZombieGameState::INGAME) return;

	if (GameInstance->PlayerPawns.IsEmpty()) return;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	TArray<AActor*> ignoredActors;
	ignoredActors.Add(GetOwner());

	TArray<ASpawnArea*> overlappedAreas;

	for (uint8 i = 0; i < GameInstance->PlayerPawns.Num(); i++)
	{
		if (!GameInstance->PlayerPawns[i]) return;

		TArray<AActor*> overlappedActors;

		if (bDrawDebug)
			DrawDebugSphere(GetWorld(), GameInstance->PlayerPawns[i]->GetActorLocation(), AreaSweepRange, 26, FColor::Purple, false, 2, 0, 5);

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GameInstance->PlayerPawns[i]->GetActorLocation(), AreaSweepRange,
			traceObjectTypes, ASpawnArea::StaticClass(),
			ignoredActors, overlappedActors);

		for (uint8 j = 0; j < overlappedActors.Num(); j++)
		{
			overlappedAreas.Add((ASpawnArea*)overlappedActors[j]);
		}
	}

	if (!overlappedAreas.IsEmpty()) {
		//Set status false for areas that are not in range anymore.
		for (uint8 g = 0; g < ActiveAreaSet.Num(); g++)
		{
			if (!overlappedAreas.Contains(ActiveAreaSet[g])) {
				ActiveAreaSet[g]->SetAreaStatus(false);

				//TODO Move AI Destruction to Seperate Timer/Function SpawnManager::ActiveAreaSweep()
				//Destroy AI That is no longer in active Area.
				for (uint8 j = 0; j < ActiveAreaSet[g]->AI_ActiveInArea.Num(); j++)
				{
					ACharacter* AI = ActiveAreaSet[g]->AI_ActiveInArea[j];
					Current_AI_Population--;
					ActiveAreaSet[g]->AI_ActiveInArea.Remove(AI);
					AI->Destroy();
				}
			}
		}
		ActiveAreaSet.Empty();

		//Set status to true for areas that are now in range, and add them to the array.
		for (uint8 f = 0; f < overlappedAreas.Num(); f++)
		{
			if (!ActiveAreaSet.Contains(overlappedAreas[f])) {
				ActiveAreaSet.Add(overlappedAreas[f]);
				overlappedAreas[f]->SetAreaStatus(true);

				if (bDrawDebug) {
					FVector origin;
					FVector extents;
					overlappedAreas[f]->GetActorBounds(false, origin, extents, false);
					UKismetSystemLibrary::DrawDebugBox(GetWorld(), origin, extents, FLinearColor::Green, FRotator::ZeroRotator, 1.0f, 10.0f);
				}
			}
		}
	}

	ActivePlayerArea.Empty();
	for (uint8 h = 0; h < ActiveAreaSet.Num(); h++)
	{
		if (ActiveAreaSet[h]->ContainsPlayers()) {

			ActivePlayerArea.Add(ActiveAreaSet[h]);

			if (bDrawDebug) {
				FVector origin;
				FVector extents;
				ActiveAreaSet[h]->GetActorBounds(false, origin, extents, false);
				UKismetSystemLibrary::DrawDebugBox(GetWorld(), origin, extents, FLinearColor::Red, FRotator::ZeroRotator, 2.0f, 20.0f);
			}

			ActiveAreaSet.RemoveAt(h);
		}
	}

}

void ASpawnManager::DecreaseAI_Population()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (Current_AI_Population > 0)
		Current_AI_Population--;
}


