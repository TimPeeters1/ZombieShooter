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
	AGameMode_Main();

	UGameInstance_Main* GameInstance;
	USessionSubsystem_Main* SessionSubsystem;
	int32 MaxNumberOfPlayers = 4;
	uint8 PlayersInPreGame;

	void On_PreGame_AllPlayersTravelled();

	APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	virtual void HandleSeamlessTravelPlayer(AController*& Controller) override;

public:
	APawn* SpawnGamePawn(AController* Controller);
	void RespawnGamePawn(AController* Controller);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Lobby"), Category = "GameState")
		void StartLobby(bool isLANSession);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Game"), Category = "GameState")
		void StartGame();

	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* ExitingPlayer) override;

};

