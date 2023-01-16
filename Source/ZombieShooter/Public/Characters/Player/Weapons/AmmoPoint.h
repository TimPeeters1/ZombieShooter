// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"

#include "Interaction/GenericInteractionActor.h"

#include "Characters/Player/InteractableObjectInterface.h"
#include "Engine/TriggerBox.h"

#include "AmmoPoint.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AAmmoPoint : public AGenericInteractionActor
{
	GENERATED_BODY()

private:
	AAmmoPoint();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Visuals", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

public:
	/** Returns Current StaticMesh **/
	class UStaticMeshComponent* GetMesh() { return Mesh; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AmmoPoint")
	void AddAmmoToActor(AActor* ActorToAdd);
	void AddAmmoToActor_Implementation(AActor* ActorToAdd);

	virtual void OnStartInteract_RPC_Implementation(AActor* InstigatingActor) override;

	virtual void StartHover_RPC_Implementation(AActor* InstigatingActor) override;
	virtual void StopHover_RPC_Implementation(AActor* InstigatingActor) override;
	
	/*
	UFUNCTION()
	void OnStartOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	*/
};
