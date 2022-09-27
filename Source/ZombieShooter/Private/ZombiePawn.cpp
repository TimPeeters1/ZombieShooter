


#include "ZombiePawn.h"

// Sets default values
AZombiePawn::AZombiePawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = MaxHealth;
}

// Called when the game starts or when spawned
void AZombiePawn::BeginPlay()
{
	Super::BeginPlay();

}
	

float AZombiePawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health)

		if (Health <= 0)
			UE_LOG(LogTemp, Warning, TEXT("Death!"));

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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



