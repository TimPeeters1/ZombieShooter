// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Characters/Player/InteractableObjectInterface.h"

#include "GenericInteractionActor.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AGenericInteractionActor : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()
	
public:	
	AGenericInteractionActor();

	/*
	* Text Displayed to player when hovering over object.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString ObjectHoverText = "Press 'E' to Interact";

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void OnInteract(AActor* InstigatingActor);
	virtual void OnInteract_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void StartHover(AActor* InstigatingActor);
	virtual void StartHover_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StopHover(AActor* InstigatingActor);
	virtual void StopHover_Implementation(AActor* InstigatingActor) override;

	virtual void BeginPlay() override;
};
