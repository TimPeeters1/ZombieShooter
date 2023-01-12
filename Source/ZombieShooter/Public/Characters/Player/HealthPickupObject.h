// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "Interaction/GenericInteractionActor.h"
#include "HealthPickupObject.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AHealthPickupObject : public AGenericInteractionActor
{
	GENERATED_BODY()

public:
	AHealthPickupObject();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh;
	

};
