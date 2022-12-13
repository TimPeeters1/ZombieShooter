#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Player/InteractableObjectInterface.h"

#include "RepairItem.generated.h"

UCLASS()
class ZOMBIESHOOTER_API ARepairItem : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()

public:
	ARepairItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ObjectMesh;

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
	UFUNCTION(NetMulticast, unreliable)
		void MC_ServerInteract();
		void MC_ServerInteract_Implementation();
};
