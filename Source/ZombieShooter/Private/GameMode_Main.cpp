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
		GameInstance->PlayerCharacters.Add(NewPlayer);
}

void AGameMode_Main::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);

	if (GameInstance) {
		if (GameInstance->PlayerCharacters.Contains(ExitingPlayer))
			GameInstance->PlayerCharacters.Remove(ExitingPlayer);
	}
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

	APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnTransform);
	Controller->Possess(PlayerPawn);

	if (!bOverrideConnectionFlow)
		OnPlayerPawnSpawned(PlayerPawn);

	return PlayerPawn;
}

void AGameMode_Main::RespawnGamePawn(AController* Controller)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, Controller->GetName());
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
}

void AGameMode_Main::StartGame()
{
	GameInstance->SetGameState(EZombieGameState::PRE_GAME);

	//Travel to GameMap
	bUseSeamlessTravel = true;
	const FString URL = GameLevel.ToString();
	GetWorld()->ServerTravel(URL, false, false);
}

void AGameMode_Main::OnPlayerPawnSpawned(APawn* NewPawn)
{
	if (NumTravellingPlayers == 0)
		if (GameInstance->SpawnManager)
			GameInstance->SpawnManager->StartSpawningRoutines(0.2f);

}



