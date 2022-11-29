#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "GameFramework/GameMode.h"

#include "GameInstance_Main.h"
#include "PlayerPawn.h"
#include "SessionSubsystem_Main.h"

#include "GameMode_Main.generated.h"

class ASpawnManager;



UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AGameMode_Main : public AGameMode
{
	GENERATED_BODY()

public:
	UGameInstance_Main* GameInstance;

	UPROPERTY(Category = "Spawning", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ASpawnManager* SpawnManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		FName MenuLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		FName LobbyLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		FName GameLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
		TArray<APawn*> PlayerPawnsInGame;

	/*
	* Overrides the server connection flow, and spawns the playerpawn instantly as with a default gamemode.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		bool bOverrideConnectionFlow;
protected:
	AGameMode_Main();

	USessionSubsystem_Main* SessionSubsystem;
	int32 MaxNumberOfPlayers = 4;

	APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	virtual void HandleSeamlessTravelPlayer(AController*& Controller) override;

public:
	APawn* SpawnGamePawn(AController* Controller);
	void RespawnGamePawn(AController* Controller);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Lobby"), Category = "GameState")
		void StartLobby(bool isLANSession);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Game"), Category = "GameState")
		void StartGame();

	void OnPlayerPawnSpawned(APawn* NewPawn);

	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* ExitingPlayer) override;

};

