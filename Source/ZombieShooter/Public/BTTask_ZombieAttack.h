

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "ZombiePawn.h"

#include "BTTask_ZombieAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UBTTask_ZombieAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ZombieAttack();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};


