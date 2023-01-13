// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

#include "Interaction/GenericInteractionActor.h"

#include "HealthPickupObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthSpawned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthPickup);

/**
 * 
 */
UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AHealthPickupObject : public AGenericInteractionActor
{
	GENERATED_BODY()

private:
	AHealthPickupObject();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	virtual void BeginPlay() override;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float HealthToAdd = 50;

	/** Returns Current StaticMesh **/
	class UStaticMeshComponent* GetMesh() { return Mesh; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AmmoPoint")
		void AddHealthToActor(AActor* ActorToAdd);
	void AddHealthToActor_Implementation(AActor* ActorToAdd);

	virtual void OnStartInteract_BP_Implementation(AActor* InstigatingActor) override;

	virtual void StartHover_BP_Implementation(AActor* InstigatingActor) override;
	virtual void StopHover_BP_Implementation(AActor* InstigatingActor) override;


	UPROPERTY()
		FOnHealthSpawned OnHealthSpawned;

	UPROPERTY()
		FOnHealthPickup OnHealthPickup;

};
