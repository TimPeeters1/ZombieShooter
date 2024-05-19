#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interaction/GenericInteractionActor.h"

#include "Characters/Player/InteractableObjectInterface.h"
#include "GameObjective/RepairData.h"

#include "RepairObject.generated.h"


UCLASS()
class ZOMBIESHOOTER_API ARepairObject : public AGenericInteractionActor
{
	GENERATED_BODY()

public:
	ARepairObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		URepairData* ObjectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ObjectMesh;

protected:
	virtual void OnStartInteract_RPC_Implementation(AActor* InstigatingActor) override;

	virtual void StartHover_RPC_Implementation(AActor* InstigatingActor) override;

	virtual void StopHover_RPC_Implementation(AActor* InstigatingActor) override;

public:
	UFUNCTION(NetMulticast, Reliable)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void Dequip();
	void Dequip_Implementation();
};
