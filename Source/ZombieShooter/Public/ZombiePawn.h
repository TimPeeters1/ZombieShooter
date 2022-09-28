

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GenericHealthComponent.h"

#include "ZombiePawn.generated.h"


UCLASS()
class ZOMBIESHOOTER_API AZombiePawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombiePawn();

protected:
	/*Health*/
	//Health Component
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UGenericHealthComponent* HealthComponent;

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	virtual void BeginPlay() override;


public:

	void OnDeath();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
