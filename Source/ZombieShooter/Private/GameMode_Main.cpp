#include "GameMode_Main.h"



void AGameMode_Main::OnMatchStateSet()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, GetMatchState().ToString());
}

void AGameMode_Main::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	PlayerCharacters.Add(NewPlayer);
}

void AGameMode_Main::Logout(AController* ExitingPlayer)
{
	if (PlayerCharacters.Contains(ExitingPlayer))
		PlayerCharacters.Remove(ExitingPlayer);

	Super::Logout(ExitingPlayer);
}


void AGameMode_Main::RespawnPlayer(AController* PlayerToRespawn)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, PlayerToRespawn->GetName());
}

