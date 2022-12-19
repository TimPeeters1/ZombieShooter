

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UInteractableObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Generic Interface for interaction implementation used by APlayerPawn
 */
class ZOMBIESHOOTER_API IInteractableObjectInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartHover(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StopHover(AActor* Instigator);
};
