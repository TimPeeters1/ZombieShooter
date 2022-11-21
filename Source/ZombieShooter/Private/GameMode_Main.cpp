#include "GameMode_Main.h"

void AGameMode_Main::BeginPlay()
{
	Super::BeginPlay();

	SetGameState(EZombieGameState::DEFAULT);
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
	APawn* TempPawn = GetWorld()->SpawnActor<APawn>(APawn::StaticClass(), FTransform(FRotator::ZeroRotator, FVector::Zero(), FVector::One()));
	return TempPawn;
}

EZombieGameState AGameMode_Main::SetGameState(EZombieGameState newState)
{
	CurrentGameState = newState;
	HandleGameState();
	return newState;
}

EZombieGameState AGameMode_Main::GetGameState() const
{
	return CurrentGameState;
}

void AGameMode_Main::HandleGameState()
{
	switch (CurrentGameState)
	{
	case EZombieGameState::DEFAULT:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "DEFAULT STATE");
		break;
	case EZombieGameState::PRE_LOBBY:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "PRE LOBBY STATE");

		SetGameState(EZombieGameState::LOBBY);

		break;
	case EZombieGameState::LOBBY:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "LOBBY STATE");
		break;
	case EZombieGameState::PRE_GAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "INGAME STATE");

		SetGameState(EZombieGameState::INGAME);

		break;
	case EZombieGameState::INGAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "INGAME STATE");
		break;
	case EZombieGameState::POSTGAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "POSTGAME STATE");
		break;
	default:
		break;
	}
}

void AGameMode_Main::SpawnPlayer(AController* PlayerToSpawn)
{
	
}

void AGameMode_Main::RespawnPlayer(AController* PlayerToRespawn)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, PlayerToRespawn->GetName());
}

void AGameMode_Main::StartLobby()
{
	if (UKismetSystemLibrary::IsServer(GetWorld()))
		SetGameState(EZombieGameState::PRE_LOBBY);
}

void AGameMode_Main::StartGame()
{
	if (UKismetSystemLibrary::IsServer(GetWorld()))
		SetGameState(EZombieGameState::PRE_GAME);
}


