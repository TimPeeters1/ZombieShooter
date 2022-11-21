

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerPawn.h"

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

UCLASS()
class ZOMBIESHOOTER_API AGameMode_Main : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		EZombieGameState CurrentGameState;

	UPROPERTY(Category = "Players", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<AController*> PlayerCharacters;

	UPROPERTY(Category = "Spawning", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ASpawnManager* SpawnManager;

protected:
	//int32 MaxNumberOfPlayers = 4;

	APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetGameState"), Category = "GameState")
	EZombieGameState SetGameState(EZombieGameState newState);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetGameState"), Category = "GameState")
	EZombieGameState GetGameState() const;

	void HandleGameState();

	void SpawnPlayer(AController* PlayerToSpawn);
	void RespawnPlayer(AController* PlayerToRespawn);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StartLobby"), Category = "GameState")
		void StartLobby();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StartGame"), Category = "GameState")
		void StartGame();



	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* ExitingPlayer) override;

};

