

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
 * 
 */
class ZOMBIESHOOTER_API IInteractableObjectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
	void OnInteract();
	
};
