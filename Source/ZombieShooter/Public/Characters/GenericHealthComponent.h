

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "GenericHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageEvent);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZOMBIESHOOTER_API UGenericHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UGenericHealthComponent();

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, Category = "Health", EditAnywhere, BlueprintReadWrite)
		float MaxHealth = 100;
	UPROPERTY(ReplicatedUsing = OnRep_Health, Category = "Health", EditAnywhere, BlueprintReadWrite)
		float Health;

protected:
	bool bActorDied;

public:
	/** Returns Health Status **/
	bool IsActorDead() { return bActorDied; }

	UFUNCTION(BlueprintCallable, Category = "HealthFunctions")
	void AddHealth(float Addition);

	UFUNCTION(BlueprintCallable, Category = "HealthFunctions")
	void ReduceHealth(float Deduction);

	UFUNCTION(NetMulticast, Reliable)
	void RPC_Death();
	void RPC_Death_Implementation();

	virtual void BeginPlay() override;

	UFUNCTION()
		void OnRep_MaxHealth();
	UFUNCTION()
		void OnRep_Health();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
	FOnDeathEvent OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
	FOnDamageEvent OnTakeDamage;
};
