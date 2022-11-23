#include "GameMode_Main.h"

#include "GameInstance_Main.h"

AGameMode_Main::AGameMode_Main() {
	GameInstance = Cast<UGameInstance_Main>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
		SessionSubsystem = GameInstance->GetSubsystem<USessionSubsystem_Main>();
}

void AGameMode_Main::BeginPlay()
{
	Super::BeginPlay();

}

void AGameMode_Main::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	PlayerCharacters.Add(NewPlayer);
}

void AGameMode_Main::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);

	if (PlayerCharacters.Contains(ExitingPlayer))
		PlayerCharacters.Remove(ExitingPlayer);
}

APawn* AGameMode_Main::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
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

void AGameMode_Main::HandleSeamlessTravelPlayer(AController*& Controller)
{
	Super::HandleSeamlessTravelPlayer(Controller);
}


APawn* AGameMode_Main::SpawnGamePawn(AController* Controller)
{
	FTransform SpawnTransform = FindPlayerStart(Controller)->GetTransform();

	APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnTransform);
	Controller->Possess(PlayerPawn);

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


void AGameMode_Main::On_PreGame_AllPlayersTravelled()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "All Travelled!");
}

