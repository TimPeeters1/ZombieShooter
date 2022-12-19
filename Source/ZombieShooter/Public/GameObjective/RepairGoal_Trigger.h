// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "Characters/Player/InteractableObjectInterface.h"
#include "Components/BoxComponent.h"

#include "RepairGoal_Trigger.generated.h"

class UTextRenderComponent;
class URepairData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRepairedObject)

UCLASS()
class ZOMBIESHOOTER_API ARepairGoal_Trigger : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()

public:
	ARepairGoal_Trigger();

	/*
	* Text Displayed to player when hovering over object.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString ObjectHoverText = "Press 'E' to Repair";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		URepairData* ObjectData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ObjectMesh;

	bool bRepaired;

protected:
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollision;
	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* RepairObjectText;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void OnInteract(AActor* InstigatingActor);
	virtual void OnInteract_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void StartHover(AActor* InstigatingActor);
	virtual void StartHover_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void StopHover(AActor* InstigatingActor);
	virtual void StopHover_Implementation(AActor* InstigatingActor) override;

	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
		FOnRepairedObject OnRepaired;

public:
	UFUNCTION(NetMulticast, Reliable)
		void OnRepairedObject(APawn* InstigatingActor);
	void OnRepairedObject_Implementation(APawn* InstigatingActor);

	virtual void BeginPlay() override;
};
