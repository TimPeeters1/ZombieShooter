// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/InteractableObjectInterface.h"
#include "Engine/TriggerBox.h"
#include "AmmoPoint.generated.h"

UCLASS()
class ZOMBIESHOOTER_API AAmmoPoint : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()

private:
	AAmmoPoint();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Visuals", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	/*
	* Text Displayed to player when hovering over object.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString ObjectHoverText = "Press 'E' to Restock Ammo";

public:
	/** Returns Current StaticMesh **/
	class UStaticMeshComponent* GetMesh() { return Mesh; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AmmoPoint")
	void AddAmmoToActor(AActor* ActorToAdd);
	void AddAmmoToActor_Implementation(AActor* ActorToAdd);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void OnInteract(AActor* InstigatingActor);
	virtual void OnInteract_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void StartHover(AActor* InstigatingActor);
	virtual void StartHover_Implementation(AActor* InstigatingActor) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StopHover(AActor* InstigatingActor);
	virtual void StopHover_Implementation(AActor* InstigatingActor) override;

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
