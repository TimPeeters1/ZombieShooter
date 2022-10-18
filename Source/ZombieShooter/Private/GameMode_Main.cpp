#include "GameMode_Main.h"

void AGameMode_Main::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	PlayerCharacters.Add(NewPlayer);
}
