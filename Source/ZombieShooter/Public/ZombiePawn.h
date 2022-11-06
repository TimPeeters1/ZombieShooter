

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MetasoundSource.h"
#include "GenericHealthComponent.h"

#include "ZombiePawn.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZombieDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZombieDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZombieAttack);

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AZombiePawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombiePawn();

	/** The event delegates that other objects can subscribe to. */
	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
	FOnZombieDamaged ZombieDamaged;

	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
	FOnZombieDeath ZombieDeath;

	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
	FOnZombieAttack ZombieAttack;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackSphereSize = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackDelay = 1.5f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings|Debug", meta = (AllowPrivateAcces = "true"))
		TEnumAsByte<EDrawDebugTrace::Type> Attack_DebugType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings|Debug", meta = (AllowPrivateAcces = "true"))
		float Attack_DebugDrawTime = 5.0f;

	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> AttackAnimation;

	UPROPERTY(Category = "Sounds", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UMetaSoundSource*> AttackSounds;

	/*Health*/
	//Health Component
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UGenericHealthComponent* HealthComponent;

	//TEMP
	UPROPERTY(Category = "VFX|Temp", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* BloodSplatter_VFX;

	void OnDeath();

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	virtual void BeginPlay() override;


public:
	FTimerHandle ZombieAttackTraceDelay;

	//Called via BTTask_ZombieAttack
	UFUNCTION(NetMulticast, Reliable)
	void OnZombieAttack();
	void OnZombieAttack_Implementation();

	//Called Via OnZombieAttack
	void ZombieAttack_Trace();

	UFUNCTION(NetMulticast, unreliable)
	void MC_TakeDamageFX(FVector ImpactLocation, FVector ImpactNormal);
	void MC_TakeDamageFX_Implementation(FVector ImpactLocation, FVector ImpactNormal);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

};
