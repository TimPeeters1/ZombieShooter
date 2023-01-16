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
	UPROPERTY(ReplicatedUsing = OnRep_RepairStatus, BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		bool bRepaired;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* RepairProgressText;

	virtual void OnStartInteract_RPC_Implementation(AActor* InstigatingActor) override;

	virtual void StartHover_RPC_Implementation(AActor* InstigatingActor) override;

	virtual void StopHover_RPC_Implementation(AActor* InstigatingActor) override;

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION()
	void TextVisualUpdate();

	UFUNCTION()
	void OnRep_RepairStatus();

public:
	void OnRepairedVehicle(APawn* InstigatingActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "RepairObject")
		void StartVehicle();
};
