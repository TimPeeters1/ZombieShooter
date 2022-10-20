

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


#include "AI_Controller_Base.generated.h"

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AAI_Controller_Base : public AAIController
{
	GENERATED_BODY()

public:
	AAI_Controller_Base(FObjectInitializer const& ObjectInitializer);

protected:

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
