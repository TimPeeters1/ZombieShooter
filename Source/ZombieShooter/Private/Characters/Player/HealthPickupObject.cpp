// Project 'Zombie Shooter' -Tim Peeters 2022/23


#include "Characters/Player/HealthPickupObject.h"

AHealthPickupObject::AHealthPickupObject()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
}
