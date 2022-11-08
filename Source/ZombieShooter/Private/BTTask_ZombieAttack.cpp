#include "BTTask_ZombieAttack.h"


UBTTask_ZombieAttack::UBTTask_ZombieAttack()
{
	bNotifyTick = false;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
	NodeName = TEXT("Zombie Attack");
}

EBTNodeResult::Type UBTTask_ZombieAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!UKismetSystemLibrary::IsServer(GetWorld()))
		return EBTNodeResult::Aborted;

	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AZombiePawn* ZombieOwner = Cast<AZombiePawn>(AIController->GetPawn())) {
		ZombieOwner->OnZombieAttack();
		return EBTNodeResult::Succeeded;
	}
	else {
		return EBTNodeResult::Failed;
	}
}




