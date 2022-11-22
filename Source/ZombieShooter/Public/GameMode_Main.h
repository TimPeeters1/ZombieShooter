#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "GameFramework/GameMode.h"
#include "PlayerPawn.h"
#include "SessionSubsystem_Main.h"

#include "GameMode_Main.generated.h"

class ASpawnManager;

UENUM(BlueprintType)
enum class EZombieGameState : uint8
{
	DEFAULT = 0 UMETA(DisplayName = "DEFAULT"),
	PRE_LOBBY = 1 UMETA(DisplayName = "PreLobby"),
	LOBBY = 2 UMETA(DisplayName = "Lobby"),
	PRE_GAME = 3 UMETA(DisplayName = "PreGame"),
	INGAME = 4 UMETA(DisplayName = "InGame"),
	POSTGAME = 5 UMETA(DisplayName = "PostGame"),
};

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AGameMode_Main : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
		 EZombieGameState CurrentGameState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		 FName MenuLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		 FName LobbyLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		 FName GameLevel;

	UPROPERTY(Category = "Players", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<AController*> PlayerCharacters;

	UPROPERTY(Category = "Spawning", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ASpawnManager* SpawnManager;

protected:
	USessionSubsystem_Main* SessionSubsystem;

	int32 MaxNumberOfPlayers = 4;

	UPROPERTY(Category = "Status", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 ActivePlayersInSession;

	APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

	virtual void HandleSeamlessTravelPlayer(AController*& Controller) override;

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set GameState"), Category = "GameState")
	void SetGameState(EZombieGameState newState);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get GameState"), Category = "GameState")
	EZombieGameState GetGameState() const;

	void HandleGameState();

	void SpawnGamePawn(AController* Controller);
	void RespawnGamePawn(AController* Controller);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Lobby"), Category = "GameState")
		void StartLobby(bool isLANSession);

	UFUNCTION()
	void OnCreatedLobby(bool Succeeded);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Game"), Category = "GameState")
		void StartGame();

	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* ExitingPlayer) override;

};

