
#include "Characters/Player/Weapons/PlayerWeaponComponent.h"

#include "Characters/Player/PlayerPawn.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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
	bIsFiring = false;

	SpawnStartWeapons();
}

#pragma region WeaponObjectFunctionality 
void UPlayerWeaponComponent::SpawnStartWeapons()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	for (uint8 i = 0; i < StartingWeapons.Num(); i++)
	{
		//AWeaponObject* NewWeaponObject = (AWeaponObject*)GetWorld()->SpawnActor(AWeaponObject::StaticClass(), Params);
		AWeaponObject* NewWeaponObject = SpawnWeaponObject(StartingWeapons[i]);

		EquippedWeapons.Add(NewWeaponObject);
	}

	ActiveWeapon = EquippedWeapons[0];
	OnRep_ActiveWeapon();
}

AWeaponObject* UPlayerWeaponComponent::SpawnWeaponObject(UWeaponData* WeaponData)
{
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
	AWeaponObject* NewWeaponObject = (AWeaponObject*)GetWorld()->SpawnActorDeferred<AWeaponObject>(AWeaponObject::StaticClass(),
		SpawnTransform, GetOwner(), GetOwner()->GetInstigator());

	NewWeaponObject->SetReplicates(true);
	NewWeaponObject->WeaponData = WeaponData;
	NewWeaponObject->OnWeaponSpawned.AddDynamic(NewWeaponObject, &AWeaponObject::Equip);

	UGameplayStatics::FinishSpawningActor(NewWeaponObject, NewWeaponObject->GetTransform());

	return NewWeaponObject;
}

#pragma endregion  

#pragma region WeaponSwitchFunctionality
/*
Weapon Switching (Inventory) Functionality
*/
void UPlayerWeaponComponent::SetEquippedWeapon_Request_Implementation(uint8 Index)
{
	if (EquippedWeapons.IsEmpty()) { return; }
	if (!EquippedWeapons[Index]) { return; }

	if (ActiveWeapon != EquippedWeapons[Index] && !bIsFiring) {
		ActiveWeapon = EquippedWeapons[Index];
		bIsFiring = false;
	}

	OnRep_ActiveWeapon();
}

void UPlayerWeaponComponent::OnRep_ActiveWeapon()
{
	if (!ActiveWeapon) return;

	bIsFiring = false;

	//Set FP Visuals of ActiveWeapon
	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	if (!ParentPawn) return;

	if (ParentPawn->GetArmModel()) {
		ParentPawn->GetArmModel()->SetSkeletalMesh(ActiveWeapon->WeaponData->WeaponArmMesh, true);
	}

	if (ParentPawn->GetWeaponModel()) {
		ParentPawn->GetWeaponModel()->SetRelativeTransform(ActiveWeapon->WeaponData->FP_Model_Transform);
		ParentPawn->GetWeaponModel()->SetStaticMesh(ActiveWeapon->WeaponData->WeaponMesh);
	}

	if (ParentPawn->GetWeaponAudioComponent()) {
		ParentPawn->GetWeaponAudioComponent()->Stop();
		ParentPawn->GetWeaponAudioComponent()->SetSound(ActiveWeapon->WeaponData->ShotAudio);
	}
}

void UPlayerWeaponComponent::UpdateEquippedWeapons_Implementation()
{
	if (EquippedWeapons.IsEmpty()) {
		//Spawns Local Weapon Object for Empty Weapon, must be destroyed when equipping new item!
		FStringAssetReference AssetPath("/Game/_GAME/Blueprints/Weapons/DA_Weapon_Empty.DA_Weapon_Empty");
		UWeaponData* EmptyData = Cast<UWeaponData>(AssetPath.TryLoad());
		if (EmptyData) {
			AWeaponObject* NewWeapon = SpawnWeaponObject(EmptyData);
			EquippedWeapons.Add(NewWeapon);
			EquipPrimaryWeapon();
		}
	}
	else {
		EquipPrimaryWeapon();
	}
}

void UPlayerWeaponComponent::EquipPrimaryWeapon()
{
	SetEquippedWeapon_Request(0);
}

void UPlayerWeaponComponent::EquipSecondaryWeapon()
{
	SetEquippedWeapon_Request(1);
}

void UPlayerWeaponComponent::DropFirstWeaponFromInventory_Implementation()
{
	if (!ActiveWeapon) return;

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavigationSystem) return;

	FNavLocation DropLocation;
	if (NavigationSystem->GetRandomPointInNavigableRadius(GetOwner()->GetActorLocation(), 300.0f, DropLocation)) {

		//DrawDebugPoint(GetWorld(), DropLocation, 5.0f, FColor::Red, false, 5.0f);

		FVector Origin;
		FVector Extents;
		ActiveWeapon->GetActorBounds(true, Origin, Extents);
		DropLocation.Location += FVector(0, 0, Extents.Z);
		ActiveWeapon->SetActorLocation(DropLocation);
		ActiveWeapon->Dequip();

		if (!EquippedWeapons.IsEmpty()) {
			uint8 WeaponIndex = EquippedWeapons.Find(ActiveWeapon);
			EquippedWeapons.RemoveAt(WeaponIndex);
		}

		UpdateEquippedWeapons();
	}
}

#pragma endregion 

#pragma region FiringFunctionality 
/*
Firing Functionality
*/
void UPlayerWeaponComponent::OnFire()
{
	if (!ActiveWeapon) return;

	bIsFiring = true;

	EWeaponType ActiveType = ActiveWeapon->WeaponData->WeaponBehaviour;
	switch (ActiveType)
	{
	case EWeaponType::MELEE:
		break;
	case EWeaponType::SHOTGUN:
		break;
	case EWeaponType::SINGLEFIRE:
		SingleFire();
		break;
	case EWeaponType::BURSTFIRE:
		break;
	case EWeaponType::AUTOFIRE:
		GetOwner()->GetWorldTimerManager().SetTimer(AutomaticFireTimer, this, &UPlayerWeaponComponent::SingleFire, ActiveWeapon->WeaponData->Default_ShotDelay, true, 0.0f);
		break;
	default:
		break;
	}
}

void UPlayerWeaponComponent::OnFireEnd()
{
	if (!ActiveWeapon) return;

	bIsFiring = false;

	EWeaponType ActiveType = ActiveWeapon->WeaponData->WeaponBehaviour;
	switch (ActiveType)
	{
	case EWeaponType::MELEE:
		break;
	case EWeaponType::SHOTGUN:
		break;
	case EWeaponType::SINGLEFIRE:
		break;
	case EWeaponType::BURSTFIRE:
		break;
	case EWeaponType::AUTOFIRE:
		GetOwner()->GetWorldTimerManager().ClearTimer(AutomaticFireTimer);
		break;
	default:
		break;
	}
}

void UPlayerWeaponComponent::SingleFire()
{
	ServerFireWeapon();
	if (ActiveWeapon->LocalCurrentAmmo > 0) {
		APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());

		//Deduce from local var, to update via RepNotify! (Reduces snappy shooting over network)
		ActiveWeapon->LocalCurrentAmmo--;

		//Visuals and VFX!
		//ParentPawn->GetWeaponAudioComponent()->SetSound(ActiveWeapon->WeaponData->ShotAudio);
		ParentPawn->GetWeaponAudioComponent()->Play();

		if (ActiveWeapon->WeaponData->MuzzleFlash_VFX)
			UNiagaraFunctionLibrary::SpawnSystemAttached(ActiveWeapon->WeaponData->MuzzleFlash_VFX, ParentPawn->GetWeaponModel(), FName(TEXT("MuzzleFlash")), FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);

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

		if (ParentPawn->GetFP_Camera()) {
			FVector StartLoc = ParentPawn->GetFP_Camera()->GetComponentLocation();

			//TODO Add Gun Range
			FVector ControlRot = ParentPawn->GetControlRotation().Vector() * 10000.0f;
			FVector EndLoc = StartLoc + ControlRot;

			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(GetOwner());

			//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor(255, 0, 0), false, 2.0f, 0, 3.f);

			GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, CollisionParams);

			if (HitResult.GetActor()) {
				//FString hitRes = HitResult.GetActor()->GetName();
				//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *hitRes);

				//TODO Add Gun Damage
				UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10, ParentPawn->GetController(), GetOwner(), UDamageType::StaticClass());
			}
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
	if (bIsFiring) return;

	if (ActiveWeapon->LocalCurrentAmmo >= 0) {
		APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());

		/*
		if (ActiveWeapon->InventoryAmmo > ActiveWeapon->MagazineSize)
			ActiveWeapon->CurrentAmmo = ActiveWeapon->InventoryAmmo - ActiveWeapon->MagazineSize;
		else
			ActiveWeapon->CurrentAmmo = ActiveWeapon->InventoryAmmo;
		*/

		//Visuals and VFX!
		//ParentPawn->GetWeaponAudioComponent()->SetSound(ActiveWeapon->WeaponData->ReloadAudio);
		//ParentPawn->GetWeaponAudioComponent()->Play();
		OnReloadEvent.Broadcast();

	}
}

void UPlayerWeaponComponent::ServerReloadWeapon_Implementation()
{
	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	if (ParentPawn) {
		if (!ActiveWeapon) return;

		if (ActiveWeapon->InventoryAmmo <= 0) return;

		uint8 ammoToAdd = ActiveWeapon->MagazineSize - ActiveWeapon->CurrentAmmo;

		if (ActiveWeapon->InventoryAmmo >= ammoToAdd) {
			ActiveWeapon->CurrentAmmo += ammoToAdd;
			ActiveWeapon->InventoryAmmo -= ammoToAdd;
		}
		else {
			ActiveWeapon->CurrentAmmo = ActiveWeapon->InventoryAmmo;
			ActiveWeapon->InventoryAmmo = 0;
		}

		//Server Local Ammo
		ActiveWeapon->LocalCurrentAmmo = ActiveWeapon->CurrentAmmo;
	}
}
#pragma endregion 

