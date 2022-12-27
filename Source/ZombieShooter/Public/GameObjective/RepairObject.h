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

	/*
	* Text Displayed to player when hovering over object.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString ObjectHoverText = "Press 'E' to Pickup";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		URepairData* ObjectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ObjectMesh;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void OnInteract(AActor* InstigatingActor);
	virtual void OnInteract_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void StartHover(AActor* InstigatingActor);
	virtual void StartHover_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void StopHover(AActor* InstigatingActor);
	virtual void StopHover_Implementation(AActor* InstigatingActor) override;

public:
	UFUNCTION(NetMulticast, Reliable)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void Dequip();
	void Dequip_Implementation();
};
