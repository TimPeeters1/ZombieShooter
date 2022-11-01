

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"

#include "InteractableObjectInterface.h"
#include "RepairItem.h"

#include "Components/TextRenderComponent.h"


#include "RepairObjective.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIESHOOTER_API ARepairObjective : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()

public:
	ARepairObjective();

	//Amount of RepairItems that need to be added before the objective is completed.
	UPROPERTY(Replicated, Category = "Interaction", EditAnywhere, BlueprintReadWrite)
		uint8 RepairItemAmount = 3;

	UPROPERTY(ReplicatedUsing = OnRep_RepairAmount, Category = "Interaction", EditAnywhere, BlueprintReadOnly)
		uint8 CurrentRepairedAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<ARepairItem*> CollectedRepairItems;

protected:
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ObjectMesh;
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* RepairProgressText;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract();
		virtual void OnInteract_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartHover();
		virtual void StartHover_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StopHover();
		virtual void StopHover_Implementation() override;
	
	//Visuals
	void TextVisualUpdate();

public:
	UFUNCTION(BlueprintCallable, Category = "RepairItem")
		void AddRepairItem(ARepairItem* RepairObject);

	UFUNCTION()
		void OnRep_RepairAmount();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
