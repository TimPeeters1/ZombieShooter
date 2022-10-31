

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		uint8 RepairItemAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<ARepairItem*> CollectedRepairItems;

	UFUNCTION(BlueprintCallable, Category = "RepairItem")
		void AddRepairItem(ARepairItem* RepairObject);
};
