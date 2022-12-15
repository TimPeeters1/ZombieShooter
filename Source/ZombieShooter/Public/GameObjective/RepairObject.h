#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Characters/Player/InteractableObjectInterface.h"
#include "GameObjective/RepairData.h"

#include "RepairObject.generated.h"


UCLASS()
class ZOMBIESHOOTER_API ARepairObject : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()

public:
	ARepairObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		URepairData* ObjectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ObjectMesh;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* InteractionInstigator);
	virtual void OnInteract_Implementation(AActor* InteractionInstigator) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StartHover();
	virtual void StartHover_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StopHover();
	virtual void StopHover_Implementation() override;

public:
	UFUNCTION(NetMulticast, unreliable)
		void InteractRPC();
	void InteractRPC_Implementation();
};
