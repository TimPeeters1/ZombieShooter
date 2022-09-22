
#include "WeaponComponent_Base.h"

#include "PlayerPawn.h"


void UWeaponComponent_Base::SetEquippedWeapon(UWeaponData* NewWeapon)
{
	EquippedWeapon = NewWeapon;

	this->SetStaticMesh(NewWeapon->WeaponModel);
	this->SetRelativeTransform(EquippedWeapon->FP_Model_Transform);
};

void UWeaponComponent_Base::Server_FireWeapon_Implementation() 
{
	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	FHitResult HitResult(ForceInit);

	if (ParentPawn) {
		//TODO Add Gun Range
		FVector StartLoc = ParentPawn->FP_PlayerCamera->GetComponentLocation();
		FVector ControlRot = ParentPawn->GetControlRotation().Vector();
		FVector EndLoc = StartLoc + ControlRot;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ParentPawn->GetControlRotation().ToString());

		FCollisionQueryParams CollisionParams;

		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor(255, 0, 0), false, 2.0f, 0, 3.f);

		//GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, CollisionParams);

		//TODO Add Gun Damage
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10, ParentPawn->GetController(), GetOwner(), UDamageType::StaticClass());
	}
}

bool UWeaponComponent_Base::Server_FireWeapon_Validate() {
	return true;
}

void UWeaponComponent_Base::ReloadWeapon()
{
}
