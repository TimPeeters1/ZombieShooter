

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
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
	virtual void OnStartInteract(AActor* Instigator);
	virtual	void OnStopInteract(AActor* Instigator);

	virtual	void StartHover(AActor* Instigator);
	virtual	void StopHover(AActor* Instigator);

};
