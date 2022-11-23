

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstance_Main.generated.h"

UENUM(BlueprintType)
enum class EZombieGameState : uint8
{
	DEFAULT = 0 UMETA(DisplayName = "DEFAULT"),
	LOBBY = 2 UMETA(DisplayName = "Lobby"),
	PRE_GAME = 3 UMETA(DisplayName = "PreGame"),
	INGAME = 4 UMETA(DisplayName = "InGame"),
	POSTGAME = 5 UMETA(DisplayName = "PostGame"),
};

/**
 *
 */
UCLASS(Blueprintable)
class ZOMBIESHOOTER_API UGameInstance_Main : public UGameInstance
{
	GENERATED_BODY()

protected:
		EZombieGameState CurrentGameState;
		uint8 ActivePlayersInSession;

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set GameState"), Category = "GameState")
		void SetGameState(EZombieGameState newState);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get GameState"), Category = "GameState")
		EZombieGameState GetGameState() const;

	void HandleGameState();
};
