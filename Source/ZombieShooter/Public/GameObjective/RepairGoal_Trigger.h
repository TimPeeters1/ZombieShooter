// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "Interaction/GenericInteractionActor.h"

#include "Components/BoxComponent.h"

#include "RepairGoal_Trigger.generated.h"

class UTextRenderComponent;
class URepairData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRepairedObject)

UCLASS()
class ZOMBIESHOOTER_API ARepairGoal_Trigger : public AGenericInteractionActor
{
	GENERATED_BODY()

public:
	ARepairGoal_Trigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		URepairData* ObjectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ObjectMesh;

	UPROPERTY(ReplicatedUsing = OnRep_Repaired, BlueprintReadOnly, EditAnywhere)
	bool bRepaired;

protected:
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollision;
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* RepairObjectText;

	UFUNCTION()
	void OnRepairedObject(APawn* InstigatingActor);

	UFUNCTION()
	void OnRep_Repaired();

	virtual void OnStartInteract_RPC_Implementation(AActor* InstigatingActor) override;

	virtual void StartHover_RPC_Implementation(AActor* InstigatingActor) override;
	virtual void StopHover_RPC_Implementation(AActor* InstigatingActor) override;

	virtual void OnCompleteHolddown_RPC_Implementation(AActor* InstigatingActor) override;

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
		FOnRepairedObject OnRepaired;
};
