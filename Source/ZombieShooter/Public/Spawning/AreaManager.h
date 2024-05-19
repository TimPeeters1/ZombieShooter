

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AreaManager.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AAreaManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAreaManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
