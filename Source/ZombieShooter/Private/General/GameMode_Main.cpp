#include "General/GameMode_Main.h"

#include "General/GameInstance_Main.h"
#include "General/GameState_Main.h"
#include "Spawning/SpawnManager.h"


class ASpawnManager;

AGameMode_Main::AGameMode_Main() {
	GameInstance = Cast<UGameInstance_Main>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
		SessionSubsystem = GameInstance->GetSubsystem<USessionSubsystem_Main>();
}

void AGameMode_Main::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<APlayerStart> PlayerStartType;
	PlayerStartType = APlayerStart::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartType, AvailablePlayerStarts);
	ShuffleArray(AvailablePlayerStarts);

	if (bOverrideConnectionFlow) {
		GetWorldTimerManager().SetTimer(OverrideZombieSpawnTimer, this, &AGameMode_Main::OverrideZombieSpawn, 5.0f, true, 1.0f);
	}
}

void AGameMode_Main::OverrideZombieSpawn()
{
	if (GameInstance->SpawnManager) {
		GameInstance->SetGameState(EZombieGameState::INGAME);
		GameInstance->SpawnManager->StartSpawningRoutines(0.2f);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Starting Spawning Routine!");

		GetWorldTimerManager().ClearTimer(OverrideZombieSpawnTimer);
	}
	else {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "No SpawnManager Assigned!");
	}
}

FColor AGameMode_Main::PickPlayerColor_Unique()
{
	if (!AvailablePlayerColors.IsEmpty()) {
		FColor PickedColor = AvailablePlayerColors[FMath::RandRange(0, (AvailablePlayerColors.Num() - 1))];
		AvailablePlayerColors.Remove(PickedColor);
		return PickedColor;
	}
	return FColor::White;
}

void AGameMode_Main::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (GameInstance) {
		GameInstance->PlayerControllers.Add(NewPlayer);

		APlayerController_Main* PlayerController = Cast<APlayerController_Main>(NewPlayer);
		if (PlayerController) {
			PlayerController->PlayerColor = PickPlayerColor_Unique();
			PlayerController->AssignedPlayerStart = AssignPlayerStart(NewPlayer);
		}
	}
}

void AGameMode_Main::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);

	if (GameInstance) {
		if (GameInstance->PlayerControllers.Contains(ExitingPlayer))
			GameInstance->PlayerControllers.Remove(ExitingPlayer);
	}
}

APlayerStart* AGameMode_Main::AssignPlayerStart(AController* Controller)
{
	if (!AvailablePlayerStarts.IsEmpty()) {
		if (Cast<APlayerStart>(AvailablePlayerStarts[0])) {
			APlayerStart* SelectedStart = Cast<APlayerStart>(AvailablePlayerStarts[0]);
			UsedPlayerStarts.Add(SelectedStart);
			AvailablePlayerStarts.RemoveAt(0);

			return SelectedStart;
		}
		else {
			return Cast<APlayerStart>(FindPlayerStart(Controller));
		}
	}
	return Cast<APlayerStart>(FindPlayerStart(Controller));
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
		case EZombieGameState::INGAME:
			return SpawnGamePawn(NewPlayer);
		}
	}

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(APawn::StaticClass(), FTransform(FRotator::ZeroRotator, FVector::Zero(), FVector::One()));
	return NewPawn;
}

APawn* AGameMode_Main::SpawnGamePawn(AController* Controller)
{
	APlayerController_Main* PlayerController = Cast<APlayerController_Main>(Controller);
	FTransform SpawnTransform = FindPlayerStart(Controller)->GetActorTransform();

	if (PlayerController) {
		if (PlayerController->AssignedPlayerStart) {
			SpawnTransform = PlayerController->AssignedPlayerStart->GetActorTransform();
		}
	}

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnTransform);
	Controller->Possess(NewPawn);

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(NewPawn);
	if (PlayerPawn) {
		PlayersAliveInGame.Add(PlayerPawn);
		PlayerPawn->GetHealthComponent()->OnDeath.AddUniqueDynamic(this, &AGameMode_Main::OnPlayerDeath);

		if (PlayerController) {
			PlayerPawn->PlayerGameColor = PlayerController->PlayerColor;
			PlayerPawn->OnRep_PlayerColor();
		}
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

void AGameMode_Main::ShuffleArray(TArray<AActor*>& inArray)
{
	if (inArray.Num() > 0)
	{
		int32 LastIndex = inArray.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				inArray.Swap(i, Index);
			}
		}
	}
}

void AGameMode_Main::EndGameLost()
{
	GameInstance->SetGameState(EZombieGameState::POSTGAME);

	for (uint8 i = 0; i < GameInstance->PlayerControllers.Num(); i++)
	{
		APlayerController_Main* PlayerController = Cast<APlayerController_Main>(GameInstance->PlayerControllers[i]);
		if (PlayerController) {
			PlayerController->PlayerEndState = EZombieGameWinState::LOST;
		}
		//Cast<APlayerController_Main>(GameInstance->PlayerControllers[i])->PlayerEndState = EZombieGameWinState::LOST;
	}

	OnGameEnd.Broadcast();
}

void AGameMode_Main::EndGameWin(TArray<APawn*> WinningActors)
{
	GameInstance->SetGameState(EZombieGameState::POSTGAME);

	for (uint8 i = 0; i < GameInstance->PlayerControllers.Num(); i++)
	{
		APlayerController_Main* PlayerController = Cast<APlayerController_Main>(GameInstance->PlayerControllers[i]);
		if (PlayerController) {
			PlayerController->PlayerEndState = EZombieGameWinState::LOST;
		}
	}

	for (uint8 j = 0; j < WinningActors.Num(); j++)
	{
		APlayerController_Main* PlayerController = Cast<APlayerController_Main>(WinningActors[j]->GetController());
		if (PlayerController) {
			PlayerController->PlayerEndState = EZombieGameWinState::WON;
		}
	}

	OnGameEnd.Broadcast();
}

void AGameMode_Main::OnPlayerDeath()
{
	for (uint8 i = 0; i < PlayersAliveInGame.Num(); i++)
	{
		if (PlayersAliveInGame[i]->GetHealthComponent()->IsActorDead())
		{
			PlayersDeadInGame.Add(PlayersAliveInGame[i]);
			PlayersAliveInGame.RemoveAt(i);
		}
	}

	if (PlayersAliveInGame.IsEmpty())
	{
		EndGameLost();
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



