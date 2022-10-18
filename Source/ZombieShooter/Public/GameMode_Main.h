

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "GameFramework/GameModeBase.h"


#include "PlayerPawn.h"

#include "GameMode_Main.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AGameMode_Main : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = "Players", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<AController*> PlayerCharacters;

	virtual void OnPostLogin(AController* NewPlayer) override;
};
