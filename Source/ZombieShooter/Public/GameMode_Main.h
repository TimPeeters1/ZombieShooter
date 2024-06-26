#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "GameFramework/GameMode.h"

#include "GameInstance_Main.h"
#include "PlayerPawn.h"
#include "SessionSubsystem_Main.h"

#include "GameMode_Main.generated.h"

UENUM(BlueprintType)
enum class EZombieGameEndGameState : uint8
{
	UNDEFINED = 0 UMETA(DisplayName = "Undefined"),
	WON = 1 UMETA(DisplayName = "Won"),
	LOST = 2 UMETA(DisplayName = "Lost"),
};

class ASpawnManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartLobby);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndGame);

UCLASS(Blueprintable, ClassGroup = (Custom))
class ZOMBIESHOOTER_API AGameMode_Main : public AGameMode
{
	GENERATED_BODY()

public:
	UGameInstance_Main* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		FName MenuLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		FName LobbyLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		FName GameLevel;

	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
	FOnStartLobby OnLobbyStart;
	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
	FOnStartGame OnGameStart;
	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
	FOnEndGame OnGameEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
		EZombieGameEndGameState GameEndState;

	/*
	* Overrides the server connection flow, and spawns the playerpawn instantly as with a default gamemode.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		bool bOverrideConnectionFlow;

protected:
	AGameMode_Main();

	USessionSubsystem_Main* SessionSubsystem;
	int32 MaxNumberOfPlayers = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		TArray<APlayerPawn*> PlayersAliveInGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		TArray<APlayerPawn*> PlayersDeadInGame;

	APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

	FTimerHandle OverrideZombieSpawnTimer;
	void OverrideZombieSpawn();


public:
	APawn* SpawnGamePawn(AController* Controller);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Lobby"), Category = "GameState")
		void StartLobby(bool isLANSession);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Game"), Category = "GameState")
		void StartGame();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Game"), Category = "GameState")
		void EndGame(EZombieGameEndGameState EndState);

	UFUNCTION()
	void OnPlayerDeath();
	UFUNCTION()
	void OnPlayerPawnSpawned(APawn* NewPawn);

	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* ExitingPlayer) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

};



