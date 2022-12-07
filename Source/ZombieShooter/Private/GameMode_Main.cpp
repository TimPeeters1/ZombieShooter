#include "GameMode_Main.h"

#include "GameInstance_Main.h"
#include "SpawnManager.h"

class ASpawnManager;

AGameMode_Main::AGameMode_Main() {
	GameInstance = Cast<UGameInstance_Main>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
		SessionSubsystem = GameInstance->GetSubsystem<USessionSubsystem_Main>();
}

void AGameMode_Main::BeginPlay()
{
	Super::BeginPlay();

	if (bOverrideConnectionFlow)
		GetWorldTimerManager().SetTimer(OverrideZombieSpawnTimer, this, &AGameMode_Main::OverrideZombieSpawn, 5.0f, true, 1.0f);
}

void AGameMode_Main::OverrideZombieSpawn()
{
	if (GameInstance->SpawnManager) {
		GameInstance->SpawnManager->StartSpawningRoutines(0.2f);
		GetWorldTimerManager().ClearTimer(OverrideZombieSpawnTimer);
	}
}

void AGameMode_Main::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (GameInstance)
		GameInstance->PlayerControllers.Add(NewPlayer);
}

void AGameMode_Main::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);

	if (GameInstance) {
		if (GameInstance->PlayerControllers.Contains(ExitingPlayer))
			GameInstance->PlayerControllers.Remove(ExitingPlayer);
	}
}

void AGameMode_Main::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

APawn* AGameMode_Main::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if (bOverrideConnectionFlow) {
		return SpawnGamePawn(NewPlayer);
	}

	if (GameInstance) {
		switch (GameInstance->GetGameState())
		{
		case EZombieGameState::PRE_GAME:
			return SpawnGamePawn(NewPlayer);
		}
	}

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(APawn::StaticClass(), FTransform(FRotator::ZeroRotator, FVector::Zero(), FVector::One()));
	return NewPawn;
}


APawn* AGameMode_Main::SpawnGamePawn(AController* Controller)
{
	FTransform SpawnTransform = FindPlayerStart(Controller)->GetTransform();

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnTransform);
	Controller->Possess(NewPawn);

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(NewPawn);
	if (PlayerPawn){
		PlayerPawn->GetHealthComponent()->OnDeath.AddUniqueDynamic(this, &AGameMode_Main::OnPlayerDeath);
		PlayersAliveInGame.Add(PlayerPawn);
	}
	if (!bOverrideConnectionFlow)
		OnPlayerPawnSpawned(NewPawn);

	return NewPawn;
}

void AGameMode_Main::StartLobby(bool isLANSession)
{
	if (SessionSubsystem)
		SessionSubsystem->CreateSession(MaxNumberOfPlayers, isLANSession);
	else
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "No Session Subsystem!");

	const FString URL = LobbyLevel.ToString() + "?Listen";
	GetWorld()->ServerTravel(URL, true, false);

	GameInstance->SetGameState(EZombieGameState::LOBBY);

	OnLobbyStart.Broadcast();
}

void AGameMode_Main::StartGame()
{
	GameInstance->SetGameState(EZombieGameState::PRE_GAME);

	//Travel to GameMap
	bUseSeamlessTravel = true;
	const FString URL = GameLevel.ToString();
	GetWorld()->ServerTravel(URL, false, false);

	OnGameStart.Broadcast();
}

void AGameMode_Main::EndGame(EZombieGameEndGameState EndState)
{
	GameInstance->SetGameState(EZombieGameState::POSTGAME);
	GameEndState = EndState;

	OnGameEnd.Broadcast();
}

void AGameMode_Main::OnPlayerDeath()
{
	for (uint8 i = 0; i < PlayersAliveInGame.Num(); i++)
	{
		if(PlayersAliveInGame[i]->GetHealthComponent()->IsActorDead())
		{
			PlayersDeadInGame.Add(PlayersAliveInGame[i]);
			PlayersAliveInGame.RemoveAt(i);
		}
	}

	if(PlayersAliveInGame.IsEmpty())
	{
		EndGame(EZombieGameEndGameState::LOST);
	}
}

void AGameMode_Main::OnPlayerPawnSpawned(APawn* NewPawn)
{
	
	if (NumTravellingPlayers <= 0) {
			GameInstance->SetGameState(EZombieGameState::INGAME);
		if (GameInstance->SpawnManager) {
			GameInstance->SpawnManager->StartSpawningRoutines(0.2f);
		}
		else {
			GetWorldTimerManager().SetTimer(OverrideZombieSpawnTimer, this, &AGameMode_Main::OverrideZombieSpawn, 5.0f, true, 1.0f);
		}
	}

}



