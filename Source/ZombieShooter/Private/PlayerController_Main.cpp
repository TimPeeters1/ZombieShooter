#include "PlayerController_Main.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameInstance_Main.h"
#include "GameMode_Main.h"
#include "Kismet/KismetStringLibrary.h"

APlayerController_Main::APlayerController_Main() {
	GameInstance = Cast<UGameInstance_Main>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void APlayerController_Main::BeginPlay()
{
	SessionSubsystem = GameInstance->GetSubsystem<USessionSubsystem_Main>();
	if (SessionSubsystem) {
		SessionSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &APlayerController_Main::OnFoundSessions);
		SessionSubsystem->OnJoinGameSessionCompleteEvent.AddUObject(this, &APlayerController_Main::OnJoinedSession);
	}
}

void APlayerController_Main::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (GameInstance) {
		if (!GameInstance->PlayerPawns.Contains(InPawn)) {
			GameInstance->PlayerPawns.Add(InPawn);
		}
	}
}

void APlayerController_Main::OnUnPossess()
{
	Super::OnUnPossess();

	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	if (GameInstance->PlayerPawns.Contains(this->GetPawn())) {
		GameInstance->PlayerPawns.Remove(this->GetPawn());
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
		if (Successful && SessionResults.Num() > 0){
			SessionSubsystem->JoinGameSession(SessionResults[0]);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Joining Session!");
		}
		else {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "No Session(s) Found!");
		}
	}
}

void APlayerController_Main::OnJoinedSession(EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionSubsystem)
		SessionSubsystem->TryTravelToCurrentSession();
}

void APlayerController_Main::RequestRespawn_Implementation()
{
	InitRespawn();
}

void APlayerController_Main::InitRespawn()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	GetPawn()->Destroy();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, UKismetStringLibrary::Conv_BoolToString(UGameplayStatics::GetGameMode(GetWorld())->PlayerCanRestart(this)));



}

