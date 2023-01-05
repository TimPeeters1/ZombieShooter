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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionSettings")
		bool bHolddown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionSettings")
		float HoldTime = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionSettings")
	FTimerHandle HolddownTimer;

	/*
	* Text Displayed to player when hovering over object.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ObjectHoverText = "Press 'E' to Interact";

protected:
	bool bIsHolding;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void OnStartInteract(AActor* InstigatingActor);
	virtual void OnStartInteract_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void OnStopInteract(AActor* InstigatingActor);
	virtual void OnStopInteract_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void StartHover(AActor* InstigatingActor);
	virtual void StartHover_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StopHover(AActor* InstigatingActor);
	virtual void StopHover_Implementation(AActor* InstigatingActor) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
		void OnCompleteHolddown();
	void OnCompleteHolddown_Implementation();

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

