#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "Components/TextRenderComponent.h"

#include "Interaction/GenericInteractionActor.h"

#include "RepairObject.h"
#include "RepairGoal_Trigger.h"

#include "RepairGoal.generated.h"

UCLASS()
class ZOMBIESHOOTER_API ARepairGoal : public AGenericInteractionActor
{
	GENERATED_BODY()

public:
	ARepairGoal();

	UPROPERTY(Category = "RepairMechanic", EditAnywhere, BlueprintReadWrite)
		TArray<ARepairGoal_Trigger*> RequiredRepairObjects;

	UPROPERTY(Category = "RepairMechanic", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* VehicleRepairSound;

protected:
	UPROPERTY(Category = "RepairMechanic", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool bRepaired;

	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* RepairProgressText;

	virtual void OnStartInteract_BP_Implementation(AActor* InstigatingActor) override;

	virtual void StartHover_BP_Implementation(AActor* InstigatingActor) override;

	virtual void StopHover_BP_Implementation(AActor* InstigatingActor) override;

	//Visuals
	void TextVisualUpdate();

public:
	UFUNCTION(BlueprintNativeEvent, Category = "RepairObject")
		void OnRepairedObject(APawn* InstigatingActor);
	void OnRepairedObject_Implementation(APawn* InstigatingActor);

	UFUNCTION()
		void OnRep_RepairAmount();

	virtual void BeginPlay() override;
};
