#include "General/GameInstance_Main.h"

#include "General/GameState_Main.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameInstance_Main::SetZombieGameState(EZombieGameState newState)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	CurrentGameState = newState;

	AGameState_Main* GameState_Main = Cast<AGameState_Main>(GetWorld()->GetGameState());
	if (GameState_Main){
		GameState_Main->GameState_Replicated = CurrentGameState;
	}

	HandleGameState();
}

EZombieGameState UGameInstance_Main::GetZombieGameState() const
{
	return CurrentGameState;
}

void UGameInstance_Main::HandleGameState()
{
	switch (CurrentGameState)
	{
	case EZombieGameState::DEFAULT:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "DEFAULT STATE");
		break;
	case EZombieGameState::LOBBY:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "LOBBY STATE");
		break;
	case EZombieGameState::PRE_GAME:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "PRE GAME STATE");
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