#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Camera/CameraActor.h"
#include "Components/SphereComponent.h"

#include "PlayerPawn.h"

#include "PlayerDeathCamera.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIESHOOTER_API APlayerDeathCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	APlayerDeathCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		APlayerPawn* PlayerRef;
private:
	virtual void BeginPlay() override;
};
