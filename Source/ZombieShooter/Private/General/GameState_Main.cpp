#include "General/GameState_Main.h"
#include "Kismet/KismetSystemLibrary.h"


AGameState_Main::AGameState_Main()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	UGameInstance_Main* GameInstance = Cast<UGameInstance_Main>(GetGameInstance());
	if (GameInstance) {
		GameState_Replicated = GameInstance->GetGameState();
	}
}

void AGameState_Main::OnReplicatedGameState()
{
	/*
	switch (GameState_Replicated)
	{
	case EZombieGameState::DEFAULT:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "DEFAULT STATE");
		break;
	case EZombieGameState::LOBBY:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "LOBBY STATE");
		break;
	case EZombieGameState::PRE_GAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "PRE GAME STATE");
		break;
	case EZombieGameState::INGAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "INGAME STATE");
		break;
	case EZombieGameState::POSTGAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "POSTGAME STATE");
		break;
	default:
		break;
	}
	*/

	OnGameStateChanged.Broadcast();
}

void AGameState_Main::OnReplicatedEndState()
{
	OnEndStateChanged.Broadcast();
}

void AGameState_Main::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameState_Main, GameState_Replicated);
	DOREPLIFETIME(AGameState_Main, LobbyPlayerNameList);
	//DOREPLIFETIME(AGameState_Main, GameEndState_Replicated);
}
