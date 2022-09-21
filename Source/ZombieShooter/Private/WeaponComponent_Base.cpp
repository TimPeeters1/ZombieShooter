
#include "WeaponComponent_Base.h"


void UWeaponComponent_Base::SetEquippedWeapon(UWeaponData* NewWeapon)
{
	EquippedWeapon = NewWeapon;

	this->SetStaticMesh(NewWeapon->WeaponModel);
	this->SetRelativeTransform(EquippedWeapon->FP_Model_Transform);
};

void UWeaponComponent_Base::FireWeapon()
{
	FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();

	FHitResult Hit(ForceInit);
	FVector Start = CameraLocation;

	ACharacter* PlayerParent = (ACharacter*)GetOwner();

	if (PlayerParent) {
		//TODO Add Gun Range
		FVector End = Start + (UKismetMathLibrary::GetForwardVector(PlayerParent->GetControlRotation()) * 10000.0f);

		FCollisionQueryParams CollisionParams;

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false , 2.f, false, 4.f);

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldDynamic, CollisionParams);
	}
}

void UWeaponComponent_Base::ReloadWeapon()
{
}
