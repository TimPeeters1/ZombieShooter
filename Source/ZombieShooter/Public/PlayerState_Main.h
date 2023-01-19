// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/PlayerController_Main.h"
#include "GameFramework/PlayerState.h"

#include "General/GameInstance_Main.h"
#include "General/GameMode_Main.h"

#include "PlayerState_Main.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API APlayerState_Main : public APlayerState
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
		EZombieGameWinState PlayerEndState = EZombieGameWinState::UNDEFINED;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
		uint8 PlayerRemainingLives = 5;

	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere)
		FString PlayerName_Custom;

	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere)
		FColor PlayerColor;

	/*PlayerStart used by gamemode to respawn. Is only ever assigned serverside!*/
	APlayerStart* AssignedPlayerStart;

private:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void SetPlayerName_Custom(const FString& InName);
	void SetPlayerName_Custom_Implementation(const FString& InName);
	
	/** Copy properties which need to be saved in inactive PlayerState */
	virtual void CopyProperties(APlayerState* PlayerState);
};
