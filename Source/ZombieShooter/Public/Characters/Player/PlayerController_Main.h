#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSessionSettings.h"

#include "General/GameInstance_Main.h"
#include "General/SessionSubsystem_Main.h"

#include "PlayerController_Main.generated.h"

UENUM(BlueprintType)
enum class EZombieGameWinState : uint8
{
	UNDEFINED = 0 UMETA(DisplayName = "Undefined"),
	WON = 1 UMETA(DisplayName = "Won"),
	LOST = 2 UMETA(DisplayName = "Lost"),
};

/**
 *
 */
UCLASS()
class ZOMBIESHOOTER_API APlayerController_Main : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EZombieGameWinState PlayerWinState;

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
