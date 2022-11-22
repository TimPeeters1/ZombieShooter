#include "GameMode_Main.h"

#include "GameInstance_Main.h"

void AGameMode_Main::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance_Main* GameInstance = Cast<UGameInstance_Main>(UGameplayStatics::GetGameInstance(GetWorld()));
	SessionSubsystem = GameInstance->GetSubsystem<USessionSubsystem_Main>();
	if (SessionSubsystem)
		SessionSubsystem->OnCreateSessionCompleteEvent.AddUObject(this, &AGameMode_Main::OnCreatedLobby);
}

void AGameMode_Main::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	PlayerCharacters.Add(NewPlayer);

	HandleGameState();

	switch (CurrentGameState) {
	case EZombieGameState::DEFAULT:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, "DEFAULT!!!!");
		break;
	case EZombieGameState::LOBBY:
		ActivePlayersInSession++;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, "Plus one!");
		break;
	case EZombieGameState::INGAME:
		if (PlayerCharacters.Num() == ActivePlayersInSession) {
			for (size_t i = 0; i < PlayerCharacters.Num(); i++)
			{
				SpawnGamePawn(PlayerCharacters[i]);
			}
		}
		break;
	}
	
}

void AGameMode_Main::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);

	if (PlayerCharacters.Contains(ExitingPlayer))
		PlayerCharacters.Remove(ExitingPlayer);
}

APawn* AGameMode_Main::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(APawn::StaticClass(), FTransform(FRotator::ZeroRotator, FVector::Zero(), FVector::One()));
	return NewPawn;
}

void AGameMode_Main::HandleSeamlessTravelPlayer(AController*& Controller)
{
	Super::HandleSeamlessTravelPlayer(Controller);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, "HELLO I TRAVELLED SEAMLESS!");
}

void AGameMode_Main::SetGameState(EZombieGameState newState)
{
	CurrentGameState = newState;
	HandleGameState();
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


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, UEnum::GetValueAsString(CurrentGameState));
}

void AGameMode_Main::SpawnGamePawn(AController* Controller)
{
	APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, FTransform(FRotator::ZeroRotator, FVector::Zero(), FVector::One()));
	Controller->Possess(PlayerPawn);
}

void AGameMode_Main::RespawnGamePawn(AController* Controller)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, Controller->GetName());
}

void AGameMode_Main::StartLobby(bool isLANSession)
{
	if (UKismetSystemLibrary::IsServer(GetWorld())) {
		SetGameState(EZombieGameState::PRE_LOBBY);

		const FString URL = LobbyLevel.ToString() + "?Listen";
		GetWorld()->ServerTravel(URL, true, false);

		SessionSubsystem->CreateSession(MaxNumberOfPlayers, isLANSession);
	}
}

void AGameMode_Main::OnCreatedLobby(bool Succeeded)
{
	if (Succeeded) {
		SetGameState(EZombieGameState::LOBBY);
		//bUseSeamlessTravel = true;
	}
}

void AGameMode_Main::StartGame()
{
	if (UKismetSystemLibrary::IsServer(GetWorld())) {
		SetGameState(EZombieGameState::PRE_GAME);

		const FString URL = GameLevel.ToString() + "?Listen";
		GetWorld()->ServerTravel(URL, false, false);
	}
}


