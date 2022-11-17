#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "PlayerController_Main.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API APlayerController_Main : public APlayerController
{
	GENERATED_BODY()

	UFUNCTION(Server, Reliable, BlueprintCallable, meta = (DisplayName = "RequestRespawn"), Category = "Spawning")
	void RequestRespawn();
	void RequestRespawn_Implementation();

	UFUNCTION()
	void InitRespawn();
	
};
