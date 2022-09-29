


#include "BTTask_ZombieAttack.h"

UBTTask_ZombieAttack::UBTTask_ZombieAttack()
{
	bNotifyTick = false;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
	NodeName = TEXT("Zombie Attack Task");
}

EBTNodeResult::Type UBTTask_ZombieAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	APawn* OwnerPawn = AIController->GetPawn();

	FHitResult HitResult(ForceInit);

	FVector StartLoc = OwnerPawn->GetActorLocation();

	FVector ControlRot = OwnerPawn->GetActorForwardVector() * AttackDistance;
	FVector EndLoc = StartLoc + ControlRot;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerPawn);

	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Purple, false, 2.0f, 0, 3.f);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, CollisionParams);

	if (HitResult.GetActor()) {
		//FString hitRes = HitResult.GetActor()->GetName();
		//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *hitRes);

		UGameplayStatics::ApplyDamage(HitResult.GetActor(), AttackDamage, AIController, OwnerPawn, UDamageType::StaticClass());
	}

	return EBTNodeResult::Succeeded;

}


