
#include "PlayerWeaponComponent.h"

#include "PlayerPawn.h"


UPlayerWeaponComponent::UPlayerWeaponComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPlayerWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerWeaponComponent, EquippedWeapons);
	DOREPLIFETIME(UPlayerWeaponComponent, ActiveWeapon);
}

void UPlayerWeaponComponent::BeginPlay()
{
	SpawnStartWeapons();
}

#pragma region WeaponObjectFunctionality 
void UPlayerWeaponComponent::SpawnStartWeapons_Implementation()
{
	UWorld* World = GetWorld();
	if (World) {
		for (size_t i = 0; i < StartingWeapons.Num(); i++)
		{
			//AWeaponObject* NewWeaponObject = (AWeaponObject*)GetWorld()->SpawnActor(AWeaponObject::StaticClass(), Params);

			FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
			AWeaponObject* NewWeaponObject = (AWeaponObject*)World->SpawnActorDeferred<AWeaponObject>(AWeaponObject::StaticClass(),
				SpawnTransform, GetOwner(), GetOwner()->GetInstigator());

			if (NewWeaponObject) {
				NewWeaponObject->SetReplicates(true);
				NewWeaponObject->WeaponData = StartingWeapons[i];

				UGameplayStatics::FinishSpawningActor(NewWeaponObject, NewWeaponObject->GetTransform());

				NewWeaponObject->GetRootComponent()->ToggleVisibility(true);
				EquippedWeapons.Add(NewWeaponObject);
				SetEquippedWeapon(0);
			}
		}
	}
}
#pragma endregion  

#pragma region WeaponSwitchFunctionality 
/*
Weapon Switching (Inventory) Functionality
*/
void UPlayerWeaponComponent::SetEquippedWeapon_Implementation(uint8 Index)
{
	//Prevents nullptr crash
	if (EquippedWeapons.IsEmpty()) return;

	if (!EquippedWeapons[Index]) return;

	ActiveWeapon = EquippedWeapons[Index];

	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	if (!ParentPawn) return;

	if (ParentPawn->GetWeaponModel()) {
		ParentPawn->GetWeaponModel()->SetStaticMesh(EquippedWeapons[Index]->WeaponData->WeaponModel);
		ParentPawn->GetWeaponModel()->SetRelativeTransform(EquippedWeapons[Index]->WeaponData->FP_Model_Transform);
	}

	if (ActiveWeapon->WeaponData->ShotAudio) {
		ParentPawn->GetWeaponAudioComponent()->SetSound(EquippedWeapons[Index]->WeaponData->ShotAudio);
	}
}

void UPlayerWeaponComponent::EquipPrimaryWeapon()
{
	SetEquippedWeapon(0);
}

void UPlayerWeaponComponent::EquipSecondaryWeapon()
{
	SetEquippedWeapon(1);
}

#pragma endregion 

#pragma region FiringFunctionality 
/*
Firing Functionality
*/
void UPlayerWeaponComponent::OnFire()
{
	ServerFireWeapon();
	if (!ActiveWeapon) return;
	if (ActiveWeapon->LocalCurrentAmmo > 0) {
		APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());

		//Deduce from local var, to update via RepNotify! (Reduces snappy shooting over network)
		ActiveWeapon->LocalCurrentAmmo--;

		//Visuals and VFX!
		ParentPawn->GetWeaponAudioComponent()->Play();
		OnFireEvent.Broadcast();
	}
}

void UPlayerWeaponComponent::ServerFireWeapon_Implementation()
{
	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	if (ParentPawn) {
		if (!ActiveWeapon) return;

		if (ActiveWeapon->CurrentAmmo <= 0) return;

		ActiveWeapon->CurrentAmmo--;

		FHitResult HitResult(ForceInit);
		//TODO FIX STARTLOC w/ Actual Camera loc!
		FVector StartLoc = ParentPawn->GetActorLocation() + FVector(0, 0, 65.0f);

		//TODO Add Gun Range
		FVector ControlRot = ParentPawn->GetControlRotation().Vector() * 10000.0f;
		FVector EndLoc = StartLoc + ControlRot;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor(255, 0, 0), false, 2.0f, 0, 3.f);

		GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, CollisionParams);

		if (HitResult.GetActor()) {
			//FString hitRes = HitResult.GetActor()->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *hitRes);

			//TODO Add Gun Damage
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10, ParentPawn->GetController(), GetOwner(), UDamageType::StaticClass());
		}

	}
}
#pragma endregion 

#pragma region ReloadFunctionality 
/*
Reloading Functionality
*/
void UPlayerWeaponComponent::OnReloadWeapon()
{
	ServerReloadWeapon();

	if (!ActiveWeapon) return;
	if (ActiveWeapon->LocalCurrentAmmo >= 0) {
		APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());

		/*
		if (ActiveWeapon->InventoryAmmo > ActiveWeapon->MagazineSize)
			ActiveWeapon->CurrentAmmo = ActiveWeapon->InventoryAmmo - ActiveWeapon->MagazineSize;
		else
			ActiveWeapon->CurrentAmmo = ActiveWeapon->InventoryAmmo;

		//Visuals and VFX!
		//ParentPawn->GetWeaponAudioComponent()->Play();
		OnReloadEvent.Broadcast();
		*/
	}
}


void UPlayerWeaponComponent::ServerReloadWeapon_Implementation()
{
	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	if (ParentPawn) {
		if (!ActiveWeapon) return;

		if (ActiveWeapon->InventoryAmmo > ActiveWeapon->MagazineSize) {
			ActiveWeapon->CurrentAmmo = ActiveWeapon->MagazineSize;
			ActiveWeapon->InventoryAmmo -= ActiveWeapon->MagazineSize;
		}	
		else {
			ActiveWeapon->CurrentAmmo = ActiveWeapon->InventoryAmmo;
			ActiveWeapon->InventoryAmmo = 0;
		}
		
	}
}
#pragma endregion 

