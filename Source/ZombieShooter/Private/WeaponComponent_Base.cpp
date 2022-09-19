
#include "WeaponComponent_Base.h"


void UWeaponComponent_Base::SetEquippedWeapon(UWeaponData* NewWeapon)
{
	EquippedWeapon = NewWeapon;

	this->SetStaticMesh(NewWeapon->WeaponModel);
	this->SetRelativeTransform(EquippedWeapon->FP_Model_Transform);
};
