

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WeaponData.h"

#include "WeaponObject.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AWeaponObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
