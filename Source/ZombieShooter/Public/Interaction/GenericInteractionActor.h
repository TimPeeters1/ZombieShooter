// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Characters/Player/InteractableObjectInterface.h"

#include "GenericInteractionActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartHover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopHover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteraction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHold);

UCLASS()
class ZOMBIESHOOTER_API AGenericInteractionActor : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()

public:
	AGenericInteractionActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionSettings")
		bool bHolddown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionSettings")
		float HoldTime = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionSettings")
		FTimerHandle HolddownTimer;

	FTimerDelegate HolddownDelegate;

	/*
	* Text Displayed to player when hovering over object.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString ObjectHoverText = "Press 'E' to Interact";

protected:
	bool bIsHolding;

	virtual void OnStartInteract(AActor* InstigatingActor) override { OnStartInteract_RPC(InstigatingActor); };
	virtual void OnStopInteract(AActor* InstigatingActor) override { OnStopInteract_RPC(InstigatingActor); };

	virtual	void StartHover(AActor* InstigatingActor) override { StartHover_RPC(InstigatingActor); };
	virtual	void StopHover(AActor* InstigatingActor) override { StopHover_RPC(InstigatingActor); };

	UFUNCTION(NetMulticast, Reliable)
		void OnStartInteract_RPC(AActor* InstigatingActor);
	virtual void OnStartInteract_RPC_Implementation(AActor* InstigatingActor);
	UFUNCTION(NetMulticast, Reliable)
		void OnStopInteract_RPC(AActor* InstigatingActor);
	virtual void OnStopInteract_RPC_Implementation(AActor* InstigatingActor);
	UFUNCTION(NetMulticast, unreliable)
		void StartHover_RPC(AActor* InstigatingActor);
	virtual void StartHover_RPC_Implementation(AActor* InstigatingActor);
	UFUNCTION(NetMulticast, unreliable)
		void StopHover_RPC(AActor* InstigatingActor);
	virtual void StopHover_RPC_Implementation(AActor* InstigatingActor);

	UFUNCTION(NetMulticast, Reliable)
		void OnCompleteHolddown_RPC(AActor* InstigatingActor);
	virtual void OnCompleteHolddown_RPC_Implementation(AActor* InstigatingActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
		void OnStartInteract_BP(AActor* InstigatingActor);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
		void OnStopInteract_BP(AActor* InstigatingActor);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
		void StartHover_BP(AActor* InstigatingActor);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
		void StopHover_BP(AActor* InstigatingActor);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction")
		void OnCompleteHolddown_BP(AActor* InstigatingActor);

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Delegates
	UPROPERTY(BlueprintAssignable, Category = "InteractionDelegates")
		FOnStartHover OnStartHover_Event;
	UPROPERTY(BlueprintAssignable, Category = "InteractionDelegates")
		FOnStopHover OnStopHover_Event;
	UPROPERTY(BlueprintAssignable, Category = "InteractionDelegates")
		FOnInteraction OnInteraction_Event;
	UPROPERTY(BlueprintAssignable, Category = "InteractionDelegates")
		FOnHold OnHolddown_Event;
};

