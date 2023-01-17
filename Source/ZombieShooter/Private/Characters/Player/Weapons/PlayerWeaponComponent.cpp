
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

	DOREPLIFETIME(UPlayerWeaponComponent, InventoryWeapons);
	DOREPLIFETIME(UPlayerWeaponComponent, ActiveWeapon);

	DOREPLIFETIME(UPlayerWeaponComponent, LastHitResult);
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

		InventoryWeapons.Add(NewWeaponObject);
	}

	ActiveWeapon = InventoryWeapons[0];

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
void UPlayerWeaponComponent::OnStartMelee_Implementation()
{
	FireCallback();
}
/*
Weapon Switching (Inventory) Functionality
*/
void UPlayerWeaponComponent::SetEquippedWeapon_Request_Implementation(uint8 Index)
{
	if (InventoryWeapons.IsEmpty()) { return; }
	if (InventoryWeapons.Num() < (Index + 1)) { return; }
	if (!InventoryWeapons[Index]) { return; }

	if (ActiveWeapon != InventoryWeapons[Index]) {
		ActiveWeapon = InventoryWeapons[Index];
		bIsFiring = false;

		//Serverside Update (RepNotify won't call on client)
		OnRep_ActiveWeapon();
	}
}

void UPlayerWeaponComponent::OnRep_ActiveWeapon()
{
	if(!ActiveWeapon){ return; }
	
	bIsFiring = false;

	UnBlockFire();

	//Set FP Visuals of ActiveWeapon
	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	if (!ParentPawn) { return; }
	
	if (ParentPawn->GetFP_WeaponModel() && ParentPawn->GetFP_ArmModel()) {
		ParentPawn->GetFP_WeaponModel()->SetStaticMesh(ActiveWeapon->WeaponData->WeaponMesh);
		ParentPawn->GetFP_WeaponModel()->SetupAttachment(ParentPawn->GetFP_ArmModel(), FName("GripPoint"));
		ParentPawn->GetFP_WeaponModel()->SetRelativeTransform(ActiveWeapon->WeaponData->WeaponMesh_Offset_FP);
	}

	if (ParentPawn->GetMesh() && ParentPawn->GetTPS_WeaponModel()) {
		ParentPawn->GetTPS_WeaponModel()->SetStaticMesh(ActiveWeapon->WeaponData->WeaponMesh);
		ParentPawn->GetTPS_WeaponModel()->SetupAttachment(ParentPawn->GetTPS_WeaponModel(), FName("GripPoint"));
		ParentPawn->GetTPS_WeaponModel()->SetRelativeTransform(ActiveWeapon->WeaponData->WeaponMesh_Offset_TPS);
	}

	OnSwitchWeapon_FP.Broadcast();
	OnSwitchWeapon_TPS.Broadcast();
}

void UPlayerWeaponComponent::OnRep_InventoryWeapons()
{
	OnWeaponInventoryChanged.Broadcast();
}

void UPlayerWeaponComponent::OnRep_HitResult()
{
	OnWeaponHit.Broadcast();
}

void UPlayerWeaponComponent::OnPickupWeapon(AWeaponObject* WeaponToPickup)
{
	
	if (InventoryWeapons.Num() == 1) {
		if (InventoryWeapons[0]->WeaponData->WeaponName == TEXT("None")) {
			AWeaponObject* ObjectToDestroy = InventoryWeapons[0];
			InventoryWeapons.Remove(ObjectToDestroy);
			ObjectToDestroy->Destroy();
			
			InventoryWeapons.Add(WeaponToPickup);
			WeaponToPickup->Equip();
			UpdateEquippedWeapons();
			
			return;
		}
	}
	if (InventoryWeapons.Num() < Max_EquippedWeapon_Count) {
		InventoryWeapons.Add(WeaponToPickup);
		WeaponToPickup->Equip();
	}
	
}

void UPlayerWeaponComponent::UpdateEquippedWeapons_Implementation()
{
	if (InventoryWeapons.IsEmpty()) {
		//Spawns Local Weapon Object for Empty Weapon, must be destroyed when equipping new item!
		FStringAssetReference AssetPath("/Game/_GAME/Blueprints/Weapons/DA_Weapon_Empty.DA_Weapon_Empty");
		UWeaponData* EmptyData = Cast<UWeaponData>(AssetPath.TryLoad());
		if (EmptyData) {
			AWeaponObject* NewWeapon = SpawnWeaponObject(EmptyData);
			InventoryWeapons.Add(NewWeapon);
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

		if (!InventoryWeapons.IsEmpty()) {
			uint8 WeaponIndex = InventoryWeapons.Find(ActiveWeapon);
			if (InventoryWeapons[WeaponIndex])
				InventoryWeapons.RemoveAt(WeaponIndex);
		}
	}

	UpdateEquippedWeapons();
}

#pragma endregion 

#pragma region FiringFunctionality 
/*
Firing Functionality
*/
void UPlayerWeaponComponent::OnFire()
{
	if (!ActiveWeapon) return;
	if (bReloading) return;

	if (bFireBlocked) return;

	BlockFire();
	
	EWeaponType ActiveType = ActiveWeapon->WeaponData->WeaponBehaviour;
	switch (ActiveType)
	{
	case EWeaponType::MELEE:
		OnFireEvent_FP.Broadcast();
		//Call to server to trigger TPS Animation via callback.
		OnStartMelee();
		break;
	case EWeaponType::SHOTGUN:
		break;
	case EWeaponType::SINGLEFIRE:
		SingleFire();
		break;
	case EWeaponType::BURSTFIRE:
		break;
	case EWeaponType::AUTOFIRE:
		GetOwner()->GetWorldTimerManager().SetTimer(AutomaticFireTimer, this, &UPlayerWeaponComponent::SingleFire, ActiveWeapon->WeaponData->ShotInterval, true, 0.0f);
		break;
	default:
		break;
	}

	bIsFiring = true;
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

void UPlayerWeaponComponent::BlockFire()
{
	if (!bFireBlocked) {
		bFireBlocked = true;
		GetOwner()->GetWorldTimerManager().SetTimer(FireDelayTimer, this, &UPlayerWeaponComponent::UnBlockFire, ActiveWeapon->WeaponData->ShotInterval, false, ActiveWeapon->WeaponData->ShotInterval);
	}
}

void UPlayerWeaponComponent::UnBlockFire()
{
	if (bFireBlocked) {
		bFireBlocked = false;
		GetOwner()->GetWorldTimerManager().ClearTimer(FireDelayTimer);
	}
}

void UPlayerWeaponComponent::FireCallback_Implementation()
{
	OnFireEvent_TPS.Broadcast();
}

void UPlayerWeaponComponent::SingleFire()
{
	ServerFireWeapon();
	
	if (ActiveWeapon->LocalCurrentAmmo > 0) {

		//Deduce from local var, to update via RepNotify! (Reduces snappy shooting over network)
		ActiveWeapon->LocalCurrentAmmo--;
		
		OnFireEvent_FP.Broadcast();
	}else
	{
		bIsFiring = false;
		OnReload();
	}
}

void UPlayerWeaponComponent::MeleeFire()
{
	if (!bIsFiring) {
		ServerMeleeWeapon();
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

			FVector ControlRot = ParentPawn->GetControlRotation().Vector() * ActiveWeapon->WeaponData->Weapon_Range;
			FVector EndLoc = StartLoc + ControlRot;

			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(GetOwner());

			//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor(255, 0, 0), false, 2.0f, 0, 3.f);

			GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, CollisionParams);

			FireCallback();

			if (HitResult.GetActor()) {
				//FString hitRes = HitResult.GetActor()->GetName();
				//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *hitRes);

				UGameplayStatics::ApplyDamage(HitResult.GetActor(), ActiveWeapon->WeaponData->Weapon_Damage, ParentPawn->GetController(), GetOwner(), UDamageType::StaticClass());
				LastHitResult = HitResult;
				
				OnWeaponHit.Broadcast();
			}
		}
	}
}

void UPlayerWeaponComponent::ServerMeleeWeapon_Implementation()
{
	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());

	if (ParentPawn) {
		const FVector StartPoint = ParentPawn->GetFP_WeaponModel()->GetSocketLocation("TraceStart");
		const FVector EndPoint = ParentPawn->GetFP_WeaponModel()->GetSocketLocation("TraceEnd");

		TArray<AActor*> ActorsToIgnore = { this->GetOwner() };

		TArray<FHitResult> HitArray;

		ETraceTypeQuery TraceChannelQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);

		const bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StartPoint, EndPoint, 50.0f,
			TraceChannelQuery, false, ActorsToIgnore,
			EDrawDebugTrace::None, HitArray, true, FLinearColor::Red, FLinearColor::Green, 5.f);

		if (Hit) {
			for (const FHitResult HitResult : HitArray)
			{
				UGameplayStatics::ApplyDamage(HitResult.GetActor(), ActiveWeapon->WeaponData->Weapon_Damage, ParentPawn->GetController(), GetOwner(), UDamageType::StaticClass());
				LastHitResult = HitResult;
				OnWeaponHit.Broadcast();
			}
		}
	}
}
#pragma endregion 

#pragma region ReloadFunctionality 
/*
Reloading Functionality
*/
void UPlayerWeaponComponent::OnReload()
{
	if (!ActiveWeapon) return;
	if (ActiveWeapon->WeaponData->WeaponBehaviour == EWeaponType::MELEE) return;
	if (bIsFiring || bReloading) return;
	
	if(ActiveWeapon->InventoryAmmo <= 0) return;
	
	GetOwner()->GetWorldTimerManager().SetTimer(ReloadTimer, this, &UPlayerWeaponComponent::ReloadWeapon, ActiveWeapon->WeaponData->ReloadTime, false, ActiveWeapon->WeaponData->ReloadTime);

	if (ActiveWeapon->LocalCurrentAmmo >= 0 && ActiveWeapon->LocalCurrentAmmo < ActiveWeapon->WeaponData->MagazineSize) {
		bReloading = true;
		
		OnReloadEvent_FP.Broadcast();
	}
}

void UPlayerWeaponComponent::ReloadWeapon()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ReloadTimer);

	ServerReloadWeapon();

	bReloading = false;
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

