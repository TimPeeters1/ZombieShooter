

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GameInstance_Main.generated.h"

class ASpawnManager;

UENUM(BlueprintType)
enum class EZombieGameState : uint8
{
	DEFAULT = 0 UMETA(DisplayName = "DEFAULT"),
	LOBBY = 1 UMETA(DisplayName = "Lobby"),
	PRE_GAME = 2 UMETA(DisplayName = "PreGame"),
	INGAME = 3 UMETA(DisplayName = "InGame"),
	POSTGAME = 4 UMETA(DisplayName = "PostGame"),
};

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API UGameInstance_Main : public UGameInstance
{
	GENERATED_BODY()

public:
	//---Player Related Variables---
	UPROPERTY(Category = "Players", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<AController*> PlayerControllers;

	UPROPERTY(Category = "Players", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<APawn*> PlayerPawns;

	//---Manager Object Variables---
	UPROPERTY(Category = "Spawning", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ASpawnManager* SpawnManager;

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
