#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SpawnObject.h"

#include "PlayerPawn.h"
#include "ZombiePawn.h"

#include "SpawnArea.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ZOMBIESHOOTER_API ASpawnArea : public ATriggerBox
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Spawning", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<ASpawnObject*> SpawnPoints;

	UPROPERTY(Category = "Spawning|AreaSettings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		uint8 DesiredEnemyCount = 5;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnEnemies"), Category = "Spawning")
		ACharacter* SpawnEnemy(TSubclassOf<class ACharacter> EnemyToSpawn);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAreaStatus"), Category = "Spawning|ActiveArea")
		bool GetAreaStatus();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetAreaStatus"), Category = "Spawning|ActiveArea")
		bool SetAreaStatus(bool bActive);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetContainsPlayers"), Category = "Spawning|ActiveArea")
		bool ContainsPlayers();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetContainsPlayers"), Category = "Spawning|ActiveArea")
		bool SetContainsPlayers(bool bActive);

private:

	UPROPERTY(Category = "Spawning|ActiveArea", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bAreaActive = false;

	UPROPERTY(Category = "Spawning|ActiveArea", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bContainsPlayers = false;

	UFUNCTION()
		void OnAreaBeginOverlap(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
		void OnAreaEndOverlap(class AActor* OverlappedActor, class AActor* OtherActor);

	void OnPlayerBeginOverlap(APlayerPawn* PlayerActor);
	void OnPlayerEndOverlap(APlayerPawn* PlayerActor);

	void OnAIBeginOverlap(AZombiePawn* AI_Actor);
	void OnAIEndOverlap(AZombiePawn* AI_Actor);

	virtual void BeginPlay() override;
public:
	UPROPERTY(Category = "Spawning|ActiveArea", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<APlayerPawn*> Players_ActiveInArea;
	UPROPERTY(Category = "Spawning|ActiveArea", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<ACharacter*> AI_ActiveInArea;
};
