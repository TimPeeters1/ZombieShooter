

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MetasoundSource.h"

#include "GenericHealthComponent.h"

#include "ZombiePawn.generated.h"


UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AZombiePawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombiePawn();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackSphereSize = 60.0f;

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
	//Called via AnimNotify
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Server_OnZombieAttack"))
	void Server_OnZombieAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MC_OnZombieAttack();
	void MC_OnZombieAttack_Implementation();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
};
