#include "Characters/ZombiePawn.h"
#include "Math/UnrealMathUtility.h"

#include "General/GameMode_Main.h"

// Sets default values
AZombiePawn::AZombiePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent = CreateDefaultSubobject<UGenericHealthComponent>("HealthComponent");
}

float AZombiePawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
		HealthComponent->ReduceHealth(DamageAmount);

	FHitResult HitRes;
	FVector ImpulseDir;

	DamageEvent.GetBestHitInfo(this, DamageCauser, HitRes, ImpulseDir);
	ImpulseDir.Normalize();

	MC_TakeDamageFX(HitRes.ImpactPoint, HitRes.ImpactNormal);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AZombiePawn::ZombieAttack_Trace()
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	FHitResult HitResult(ForceInit);

	FVector StartLoc = GetActorLocation();
	FVector ControlRot = GetActorForwardVector() * AttackDistance;
	FVector EndLoc = StartLoc + ControlRot;

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Purple, false, 2.0f, 0, 3.f);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLoc, EndLoc, AttackSphereSize, ETraceTypeQuery::TraceTypeQuery1, true,
		IgnoredActors,
		Attack_DebugType, HitResult, true,
		FLinearColor::Green, FLinearColor::Red, Attack_DebugDrawTime);

	if (HitResult.GetActor()) {
		FString hitRes = HitResult.GetComponent()->GetName();
		if (GEngine && Attack_DebugType != EDrawDebugTrace::None)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, hitRes);

		UGameplayStatics::ApplyDamage(HitResult.GetActor(), AttackDamage, GetController(), GetOwner(), UDamageType::StaticClass());
	}

	GetWorldTimerManager().ClearTimer(ZombieAttackTraceDelay);
}

void AZombiePawn::OnZombieAttack_Implementation()
{
	if (UKismetSystemLibrary::IsServer(GetWorld())) {
		GetWorldTimerManager().SetTimer(ZombieAttackTraceDelay, this, &AZombiePawn::ZombieAttack_Trace, AttackDelay, false, AttackDelay);
	}

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
		AnimInst->Montage_Play(AttackAnimation[FMath::RandRange(0, (AttackAnimation.Num() - 1))]);

}

void AZombiePawn::MC_TakeDamageFX_Implementation(FVector ImpactLocation, FVector ImpactNormal)
{
	//DrawDebugPoint(GetWorld(), ImpactLocation, 3.0f, FColor::Green, false, 0.15f);
	//TEMP Old Particle system!
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodSplatter_VFX, ImpactLocation, ImpactNormal.Rotation(), true);
}

// Called every frame
void AZombiePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombiePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombiePawn::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetMesh()->GetSocketLocation("HeadSocket");
	OutRotation = GetMesh()->GetSocketRotation("HeadSocket");
}



