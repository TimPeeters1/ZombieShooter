#include "ZombiePawn.h"
#include "Math/UnrealMathUtility.h"
// Sets default values
AZombiePawn::AZombiePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent = CreateDefaultSubobject<UGenericHealthComponent>("HealthComponent");
}

// Called when the game starts or when spawned
void AZombiePawn::BeginPlay()
{
	Super::BeginPlay();

}

float AZombiePawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(HealthComponent)
	HealthComponent->ReduceHealth(DamageAmount);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AZombiePawn::Server_OnZombieAttack()
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
}

void AZombiePawn::MC_OnZombieAttack_Implementation()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
		AnimInst->Montage_Play(AttackAnimation[FMath::RandRange(0, (AttackAnimation.Num() - 1))]);
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



