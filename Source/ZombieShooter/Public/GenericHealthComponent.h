

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericHealthComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESHOOTER_API UGenericHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGenericHealthComponent();

protected:

	UPROPERTY(Category = "Health", EditAnywhere, BlueprintReadWrite)
		float MaxHealth = 100;
	UPROPERTY(Category = "Health", EditAnywhere, BlueprintReadWrite)
		float Health;

	virtual void BeginPlay() override;

public:	

	void AddHealth(float Addition);
	void ReduceHealth(float Deduction);

	void Death();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
