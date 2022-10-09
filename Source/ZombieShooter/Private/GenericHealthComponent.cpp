


#include "GenericHealthComponent.h"

// Sets default values for this component's properties
UGenericHealthComponent::UGenericHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Health = MaxHealth;
}


// Called when the game starts
void UGenericHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGenericHealthComponent::AddHealth(float Addition)
{
	Health += Addition;
	//UE_LOG(LogTemp, Warning, TEXT("Added Health: %f"), Addition);
	if (Health > MaxHealth)
		Health = MaxHealth;
}

void UGenericHealthComponent::ReduceHealth(float Deduction)
{
	Health -= Deduction;
	//UE_LOG(LogTemp, Warning, TEXT("Reduced Health: %f"), Deduction);
	if (Health <= 0) {
		Health = 0;
		OnDeath.Broadcast();
	}
}

// Called every frame
void UGenericHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
