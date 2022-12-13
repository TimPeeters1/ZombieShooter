

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/GameplayStatics.h"
#include "General/GameMode_Main.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BTTask_TravelToActiveArea.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ZOMBIESHOOTER_API UBTTask_TravelToActiveArea : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TravelToActiveArea();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
