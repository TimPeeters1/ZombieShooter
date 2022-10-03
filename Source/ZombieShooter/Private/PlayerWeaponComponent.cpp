
#include "PlayerWeaponComponent.h"

#include "PlayerPawn.h"


void UPlayerWeaponComponent::BeginPlay()
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
				NewWeaponObject->WeaponData = StartingWeapons[i];

				UGameplayStatics::FinishSpawningActor(NewWeaponObject, NewWeaponObject->GetTransform());

				NewWeaponObject->GetRootComponent()->ToggleVisibility(true);
				EquippedWeapons.Add(NewWeaponObject);
				SetEquippedWeapon(0);
			}
		}
	}
}


void UPlayerWeaponComponent::SetEquippedWeapon(uint8 Index)
{
	if (!EquippedWeapons[Index]) return;

	ActiveWeapon = EquippedWeapons[Index];

	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	if (!ParentPawn) return;

	if (ParentPawn->GetWeaponModel()) {
		ParentPawn->GetWeaponModel()->SetStaticMesh(EquippedWeapons[Index]->WeaponData->WeaponModel);
		ParentPawn->GetWeaponModel()->SetRelativeTransform(EquippedWeapons[Index]->WeaponData->FP_Model_Transform);
	}
}

void UPlayerWeaponComponent::Server_FireWeapon_Implementation()
{
	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	FHitResult HitResult(ForceInit);

	if (ParentPawn != nullptr) {
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
			FString hitRes = HitResult.GetActor()->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *hitRes);

			//TODO Add Gun Damage
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10, ParentPawn->GetController(), GetOwner(), UDamageType::StaticClass());
		}
	}
}

bool UPlayerWeaponComponent::Server_FireWeapon_Validate() {
	return true;
}

void UPlayerWeaponComponent::ReloadWeapon()
{
}

