
#include "PlayerWeaponComponent.h"

#include "PlayerPawn.h"


void UPlayerWeaponComponent::SetEquippedWeapon(UWeaponData* NewWeapon)
{
	EquippedWeapon = NewWeapon;

	//this->SetStaticMesh(NewWeapon->WeaponModel);
	//this->SetRelativeTransform(EquippedWeapon->FP_Model_Transform);
};

void UPlayerWeaponComponent::Server_FireWeapon_Implementation()
{
	APlayerPawn* ParentPawn = Cast<APlayerPawn>(GetOwner());
	FHitResult HitResult(ForceInit);

	if (ParentPawn != nullptr) {

		//TODO FIX STARTLOC w/ Actual Camera loc!
		//FVector StartLoc = ParentPawn->GetActorLocation() + FVector(0, 0, 65.0f);

		FVector StartLoc = ParentPawn->GetActorLocation();

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ParentPawn->GetFPCamera()->GetName());

		//TODO Add Gun Range
		FVector ControlRot = ParentPawn->GetControlRotation().Vector() * 10000.0f;
		FVector EndLoc = StartLoc + ControlRot;

		FCollisionQueryParams CollisionParams;

		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor(255, 0, 0), false, 2.0f, 0, 3.f);

		GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, CollisionParams);

		//TODO Add Gun Damage
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10, ParentPawn->GetController(), GetOwner(), UDamageType::StaticClass());
	}
}

bool UPlayerWeaponComponent::Server_FireWeapon_Validate() {
	return true;
}

void UPlayerWeaponComponent::ReloadWeapon()
{
}
