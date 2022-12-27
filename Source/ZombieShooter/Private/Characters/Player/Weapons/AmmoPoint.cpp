// Project 'Zombie Shooter' -Tim Peeters 2022/23


#include "Characters/Player/Weapons/AmmoPoint.h"

#include "Characters/Player/PlayerPawn.h"
#include "Kismet/KismetSystemLibrary.h"

AAmmoPoint::AAmmoPoint()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AAmmoPoint::AddAmmoToActor_Implementation(AActor* ActorToAdd)
{
	if(!UKismetSystemLibrary::IsServer(GetWorld())) return;
	
	if(ActorToAdd)
	{
		APlayerPawn* PlayerPawn = Cast<APlayerPawn>(ActorToAdd);
		if(PlayerPawn)
		{
			TArray<AWeaponObject*> WeaponObjects = 	PlayerPawn->GetWeaponComponent()->GetEquippedWeapons();
			for (uint8 i = 0; i < WeaponObjects.Num(); ++i)
			{		
				if(	WeaponObjects[i]->InventoryAmmo < WeaponObjects[i]->MaxInventoryAmmo)
					WeaponObjects[i]->InventoryAmmo = WeaponObjects[i]->MaxInventoryAmmo;
			}
		}
	}
		
}

void AAmmoPoint::OnInteract_Implementation(AActor* InstigatingActor)
{
	IInteractableObjectInterface::OnInteract_Implementation(InstigatingActor);
	
	AddAmmoToActor(InstigatingActor);
}

void AAmmoPoint::StartHover_Implementation(AActor* InstigatingActor)
{
	IInteractableObjectInterface::StartHover_Implementation(InstigatingActor);

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStartHover(ObjectHoverText);
	}
}

void AAmmoPoint::StopHover_Implementation(AActor* InstigatingActor)
{
	IInteractableObjectInterface::StopHover_Implementation(InstigatingActor);

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InstigatingActor);
	if (PlayerPawn) {
		PlayerPawn->OnStopHover();
	}
}


