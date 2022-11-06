

#include "GenericHealthComponent.h"


// Sets default values for this component's properties
UGenericHealthComponent::UGenericHealthComponent()
{
	SetIsReplicatedByDefault(true);

	Health = MaxHealth;
}

void UGenericHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGenericHealthComponent, MaxHealth);
	DOREPLIFETIME(UGenericHealthComponent, Health);
}

// Called when the game starts
void UGenericHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGenericHealthComponent::OnReplicateDeath_Implementation()
{
	OnDeath.Broadcast();
}

void UGenericHealthComponent::AddHealth(float Addition)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	Health += Addition;
	//UE_LOG(LogTemp, Warning, TEXT("Added Health: %f"), Addition);
	if (Health > MaxHealth)
		Health = MaxHealth;
}

void UGenericHealthComponent::ReduceHealth(float Deduction)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld())) return;

	Health -= Deduction;

	//Serverside Damage Effect (TEMP)
	OnTakeDamage.Broadcast();

	//UE_LOG(LogTemp, Warning, TEXT("Reduced Health: %f"), Deduction);
	if (Health <= 0) {
		Health = 0;
		OnReplicateDeath();
	}
}

void UGenericHealthComponent::OnRep_MaxHealth()
{
}

void UGenericHealthComponent::OnRep_Health()
{
	//Cleintside Damage Effect (TEMP)
	OnTakeDamage.Broadcast();
}





