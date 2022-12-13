#include "Spawning/SpawnArea.h"
#include "Characters/AI/AI_Controller_Base.h"
#include "GameFramework/Character.h"
#include "General/GameMode_Main.h"

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
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (!Players_ActiveInArea.Contains(PlayerActor)) {
		Players_ActiveInArea.Add(PlayerActor);
		bContainsPlayers = true;
	}
}

void ASpawnArea::OnPlayerEndOverlap(APlayerPawn* PlayerActor)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (Players_ActiveInArea.Contains(PlayerActor)) {
		Players_ActiveInArea.Remove(PlayerActor);
	}

	bContainsPlayers = !Players_ActiveInArea.IsEmpty();
}

void ASpawnArea::OnAIBeginOverlap(AZombiePawn* AI_Actor)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (!AI_ActiveInArea.Contains(AI_Actor)) {
		AI_ActiveInArea.Add(AI_Actor);
	}
}

void ASpawnArea::OnAIEndOverlap(AZombiePawn* AI_Actor)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (AI_ActiveInArea.Contains(AI_Actor)) {
		AI_ActiveInArea.Remove(AI_Actor);
	}
}

ACharacter* ASpawnArea::SpawnEnemy(TSubclassOf<class ACharacter> EnemyToSpawn)
{
	if (!SpawnPoints.IsEmpty()) {
		uint8 RandomSpawnPoint = FMath::RandRange(0, (SpawnPoints.Num() - 1));
		ASpawnObject* SelectedSpawner = SpawnPoints[RandomSpawnPoint];

		if (SelectedSpawner) {
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Name = MakeUniqueObjectName(this, EnemyToSpawn, FName("Zombie_AI"));

			FRotator RandomSpawnRotation = FRotator(0, FMath::RandRange(0.0f, 359.0f), 0);

			ACharacter* NewEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyToSpawn, SelectedSpawner->GetSpawnTransform().GetLocation() + FVector(0,0,3.0), RandomSpawnRotation);
			return NewEnemy;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("SPAWNAREA %s: No SpawnObjects Found!"), *this->GetName());
	}

	//Default Implementation if SpawnObjects returns null.
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Name = MakeUniqueObjectName(this, EnemyToSpawn, FName("Zombie_AI"));
	ACharacter* NewEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyToSpawn, this->GetActorLocation(), FRotator::ZeroRotator);
	return NewEnemy;
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



