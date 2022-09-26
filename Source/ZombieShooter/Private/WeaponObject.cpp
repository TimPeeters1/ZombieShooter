


#include "WeaponObject.h"

// Sets default values
AWeaponObject::AWeaponObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

