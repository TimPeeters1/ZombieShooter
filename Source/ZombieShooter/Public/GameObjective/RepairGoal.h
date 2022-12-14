

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"

#include "Characters/Player/InteractableObjectInterface.h"
#include "GameObjective/RepairObject.h"

#include "Components/TextRenderComponent.h"


#include "RepairGoal.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIESHOOTER_API ARepairGoal : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()

public:
	ARepairGoal();

	//Amount of RepairObjects that need to be added before the objective is completed.
	UPROPERTY(Replicated, Category = "Interaction", EditAnywhere, BlueprintReadWrite)
		uint8 RepairObjectAmount = 3;

	UPROPERTY(ReplicatedUsing = OnRep_RepairAmount, Category = "Interaction", EditAnywhere, BlueprintReadOnly)
		uint8 CurrentRepairedAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<ARepairObject*> CollectedRepairObjects;

protected:
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* ObjectMesh;
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* RepairProgressText;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* InteractionInstigator);
	virtual void OnInteract_Implementation(AActor* InteractionInstigator) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartHover();
	virtual void StartHover_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StopHover();
	virtual void StopHover_Implementation() override;

	//Visuals
	void TextVisualUpdate();

public:
	UFUNCTION(BlueprintCallable, Category = "RepairObject")
		void AddRepairObject(ARepairObject* RepairObject);

	UFUNCTION()
		void OnRep_RepairAmount();

	UFUNCTION()
		void OnObjectiveRepaired();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
