

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


#include "AI_Controller_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZombieStateChanged);

UENUM(BlueprintType)
enum class E_AI_State : uint8 {
	IDLE = 0 UMETA(DisplayName = "Idle"),
	ROAMING = 1 UMETA(DisplayName = "Roaming"),
	FOLLOWING = 2 UMETA(DisplayName = "Following"),
	TRAVELLING = 3 UMETA(DisplayName = "Travelling"),
};


UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AAI_Controller_Base : public AAIController
{
	GENERATED_BODY()

public:
	AAI_Controller_Base(FObjectInitializer const& ObjectInitializer);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		E_AI_State Current_AIState;

	/** The event delegate that other objects can subscribe to. */
	UPROPERTY(BlueprintAssignable, Category = "EventDelegates")
		FOnZombieStateChanged ZombieStateChanged;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI|Sight", meta = (AllowPrivateAccess = "true"))
		float SightRadius = 2000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI|Sight", meta = (AllowPrivateAccess = "true"))
		float LoseSightRadius = 1500.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI|Sight", meta = (AllowPrivateAccess = "true"))
		float Sight_FOV = 120.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UBehaviorTree* BehaviorTree;

	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere)
		UAIPerceptionComponent* AIPerceptionComponent;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:

	UFUNCTION()
		void OnPerception(AActor* Actor, FAIStimulus Stimulus);

	class UAISenseConfig_Sight* SightPercpetion;

	void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
};
