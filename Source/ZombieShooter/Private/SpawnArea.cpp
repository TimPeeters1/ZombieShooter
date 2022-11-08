#include "SpawnArea.h"
#include "AI_Controller_Base.h"
#include "GameFramework/Character.h"
#include "GameMode_Main.h"
#include "SpawnManager.h"

void ASpawnArea::BeginPlay()
{
	OnActorBeginOverlap.AddDynamic(this, &ASpawnArea::OnAreaBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ASpawnArea::OnAreaEndOverlap);
}


void ASpawnArea::OnAreaBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (Cast<APlayerPawn>(OtherActor)) {
		OnPlayerBeginOverlap(Cast<APlayerPawn>(OtherActor));
	}
	else if (Cast<AZombiePawn>(OtherActor)) {
		OnAIBeginOverlap(Cast<AZombiePawn>(OtherActor));
	}
}

void ASpawnArea::OnAreaEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (Cast<APlayerPawn>(OtherActor)) {
		OnPlayerEndOverlap(Cast<APlayerPawn>(OtherActor));
	}
	else if (Cast<AZombiePawn>(OtherActor)) {
		OnAIEndOverlap(Cast<AZombiePawn>(OtherActor));
	}
}

void ASpawnArea::OnPlayerBeginOverlap(APlayerPawn* PlayerActor)
{
	if (!Players_ActiveInArea.Contains(PlayerActor)) {
		Players_ActiveInArea.Add(PlayerActor);
		bContainsPlayers = true;
	}
}

void ASpawnArea::OnPlayerEndOverlap(APlayerPawn* PlayerActor)
{
	if (Players_ActiveInArea.Contains(PlayerActor)) {
		Players_ActiveInArea.Remove(PlayerActor);
	}

	bContainsPlayers = !Players_ActiveInArea.IsEmpty();
}

void ASpawnArea::OnAIBeginOverlap(AZombiePawn* AI_Actor)
{
	if (!AI_ActiveInArea.Contains(AI_Actor)) {
		AI_ActiveInArea.Add(AI_Actor);
	}
}

void ASpawnArea::OnAIEndOverlap(AZombiePawn* AI_Actor)
{
	if (AI_ActiveInArea.Contains(AI_Actor)) {
		AI_ActiveInArea.Remove(AI_Actor);
	}
}

void ASpawnArea::SpawnEnemy(TSubclassOf<class ACharacter> EnemyToSpawn)
{
	if (!SpawnPoints.IsEmpty()) {
		uint8 RandomSpawnPoint = FMath::RandRange(0, (SpawnPoints.Num() - 1));
		ASpawnObject* SelectedSpawner = SpawnPoints[RandomSpawnPoint];

		if (SelectedSpawner) {
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Name = MakeUniqueObjectName(this, EnemyToSpawn, FName("Zombie_AI"));
			ACharacter* NewEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyToSpawn, SelectedSpawner->GetSpawnTransform().GetLocation(), FRotator::ZeroRotator);
			if (NewEnemy) {
				AGameMode_Main* GameMode = Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()));
				if (GameMode) {
					GameMode->SpawnManager->Current_AI_Population++;
				}
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("SPAWNAREA %s: No SpawnObjects Found!"), *this->GetName());
	}
}

bool ASpawnArea::GetAreaStatus()
{
	return bAreaActive;
}

bool ASpawnArea::SetAreaStatus(bool bActive)
{
	bAreaActive = bActive;
	return bAreaActive;
}

bool ASpawnArea::ContainsPlayers()
{
	return bContainsPlayers;
}

bool ASpawnArea::SetContainsPlayers(bool bActive)
{
	bContainsPlayers = bActive;
	return bContainsPlayers;
}



