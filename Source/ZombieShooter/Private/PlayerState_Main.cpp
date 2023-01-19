// Project 'Zombie Shooter' -Tim Peeters 2022/23


#include "PlayerState_Main.h"

void APlayerState_Main::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(APlayerState_Main, PlayerEndState);
		DOREPLIFETIME(APlayerState_Main, PlayerRemainingLives);
		DOREPLIFETIME(APlayerState_Main, PlayerName_Custom);
		DOREPLIFETIME(APlayerState_Main, PlayerColor);
}

void APlayerState_Main::SetPlayerName_Custom_Implementation(const FString& InName)
{
	PlayerName_Custom = InName;
}
