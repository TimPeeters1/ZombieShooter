

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "GenericTeamAgentInterface.h"

#include "AI_Controller_Base.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ZOMBIESHOOTER_API AAI_Controller_Base : public AAIController
{
	GENERATED_BODY()

public:
	AAI_Controller_Base();

protected:
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;

	FGenericTeamId TeamId;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	virtual void BeginPlay();

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
};
