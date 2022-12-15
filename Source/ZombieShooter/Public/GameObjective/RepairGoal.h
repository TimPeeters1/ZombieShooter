

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "Components/TextRenderComponent.h"

#include "Characters/Player/InteractableObjectInterface.h"
#include "RepairObject.h"
#include "RepairGoal_Trigger.h"

#include "RepairGoal.generated.h"

UCLASS()
class ZOMBIESHOOTER_API ARepairGoal : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()

public:
	ARepairGoal();

	UPROPERTY(Category = "RepairMechanic", EditAnywhere, BlueprintReadWrite)
		TArray<ARepairGoal_Trigger*> RequiredRepairObjects;

protected:
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* ObjectMesh;
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* RepairProgressText;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* InteractionInstigator);
	virtual void OnInteract_Implementation(AActor* InteractionInstigator) override;

	//Visuals
	void TextVisualUpdate();

public:
	UFUNCTION(BlueprintCallable, Category = "RepairObject")
		void OnRepairedObject();

	UFUNCTION()
		void OnRep_RepairAmount();

	UFUNCTION()
		void OnObjectiveRepaired();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
