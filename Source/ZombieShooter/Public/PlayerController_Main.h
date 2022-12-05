#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSessionSettings.h"

#include "GameInstance_Main.h"
#include "SessionSubsystem_Main.h"

#include "PlayerController_Main.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIESHOOTER_API APlayerController_Main : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Find Random Session"), Category = "Connection")
		void FindRandomSession(bool isLanSearch);

	void OnFoundSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);

	void OnJoinedSession(EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(Server, Reliable, BlueprintCallable, meta = (DisplayName = "RequestRespawn"), Category = "Spawning")
	void RequestRespawn();
	void RequestRespawn_Implementation();

	UFUNCTION()
		void InitRespawn();

	virtual void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

private:
	APlayerController_Main();

	UGameInstance_Main* GameInstance;
	USessionSubsystem_Main* SessionSubsystem;
};
