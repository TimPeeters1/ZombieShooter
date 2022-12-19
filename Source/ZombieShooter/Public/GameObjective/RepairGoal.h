

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

	/*
	* Text Displayed to player when hovering over object.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RepairMechanic", meta = (AllowPrivateAccess = "true"))
		FString ObjectHoverText = "Press 'E' To Enter!";

	UPROPERTY(Category = "RepairMechanic", EditAnywhere, BlueprintReadWrite)
		TArray<ARepairGoal_Trigger*> RequiredRepairObjects;

	UPROPERTY(Category = "RepairMechanic", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* VehicleRepairSound;

protected:
	UPROPERTY(Category = "RepairMechanic", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bRepaired;

	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* ObjectMesh;
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* RepairProgressText;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* InteractionInstigator);
	virtual void OnInteract_Implementation(AActor* InteractionInstigator) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void StartHover(AActor* InstigatingActor);
	virtual void StartHover_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StopHover(AActor* InstigatingActor);
	virtual void StopHover_Implementation(AActor* InstigatingActor) override;

	//Visuals
	void TextVisualUpdate();

public:
	UFUNCTION(BlueprintNativeEvent, Category = "RepairObject")
		void OnRepairedObject(APawn* InstigatingActor);
	void OnRepairedObject_Implementation(APawn* InstigatingActor);

	UFUNCTION()
		void OnRep_RepairAmount();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
