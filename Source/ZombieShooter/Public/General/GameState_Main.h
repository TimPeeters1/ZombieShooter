#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Net/UnrealNetwork.h"

#include "GameInstance_Main.h"
#include "GameMode_Main.h"

#include "GameState_Main.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStateChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndStateChanged);

/**
 *
 */
UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AGameState_Main : public AGameState
{
	GENERATED_BODY()

public:
	AGameState_Main();

	UPROPERTY(ReplicatedUsing = OnReplicatedGameState, EditAnywhere, BlueprintReadOnly)
		EZombieGameState GameState_Replicated;

	//---Player Related Variables---
	UPROPERTY(Replicated, Category = "Players", VisibleAnywhere, BlueprintReadOnly)
		FString LobbyPlayerNameList;

	//UPROPERTY(ReplicatedUsing = OnReplicatedEndState, EditAnywhere, BlueprintReadOnly)
		//EZombieGameEndGameState GameEndState_Replicated;

	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
		FOnGameStateChanged OnGameStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
		FOnEndStateChanged OnEndStateChanged;



public:
	UFUNCTION()
		void OnReplicatedGameState();

	UFUNCTION()
		void OnReplicatedEndState();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
