

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "GameFramework/GameModeBase.h"

#include "PlayerPawn.h"

#include "GameMode_Main.generated.h"

class ASpawnManager;

UCLASS()
class ZOMBIESHOOTER_API AGameMode_Main : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = "Players", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<AController*> PlayerCharacters;

	UPROPERTY(Category = "Spawning", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ASpawnManager* SpawnManager;

	virtual void OnPostLogin(AController* NewPlayer) override;
};
