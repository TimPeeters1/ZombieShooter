#include "PlayerController_Main.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameInstance_Main.h"
#include "GameMode_Main.h"

void APlayerController_Main::BeginPlay()
{
	UGameInstance_Main* GameInstance = Cast<UGameInstance_Main>(UGameplayStatics::GetGameInstance(GetWorld()));
	SessionSubsystem = GameInstance->GetSubsystem<USessionSubsystem_Main>();
	if (SessionSubsystem) {
		SessionSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &APlayerController_Main::OnFoundSessions);
		SessionSubsystem->OnJoinGameSessionCompleteEvent.AddUObject(this, &APlayerController_Main::OnJoinedSession);
	}
}


void APlayerController_Main::FindRandomSession(bool isLanSearch)
{
	if (SessionSubsystem)
		SessionSubsystem->FindSessions(1, isLanSearch);
}

void APlayerController_Main::OnFoundSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{
	if (SessionSubsystem) {
		if (Successful) {
			SessionSubsystem->JoinGameSession(SessionResults[0]);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Joining Session!");
		}
		else {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "No Session Found!");
		}
	}

}

void APlayerController_Main::OnJoinedSession(EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionSubsystem)
		SessionSubsystem->TryTravelToCurrentSession();
}

void APlayerController_Main::JoinSelectedSession()
{
}


void APlayerController_Main::RequestRespawn_Implementation()
{
	InitRespawn();
}

void APlayerController_Main::InitRespawn()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()))->RespawnPlayer(this);
}

