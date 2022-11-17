#include "PlayerController_Main.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameMode_Main.h"

void APlayerController_Main::RequestRespawn_Implementation()
{
	InitRespawn();
}

void APlayerController_Main::InitRespawn()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()))->RespawnPlayer(this);
}
