


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
	if (!UKismetSystemLibrary::IsServer(GetWorld()))
		return EBTNodeResult::Aborted;


	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	APawn* OwnerPawn = AIController->GetPawn();

	FHitResult HitResult(ForceInit);

	FVector StartLoc = OwnerPawn->GetActorLocation();

	FVector ControlRot = OwnerPawn->GetActorForwardVector() * AttackDistance;
	FVector EndLoc = StartLoc + ControlRot;

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(OwnerPawn);

	//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Purple, false, 2.0f, 0, 3.f);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLoc, EndLoc, AttackSphereSize, ETraceTypeQuery::TraceTypeQuery1, true, IgnoredActors,
		DebugType, HitResult, true, FLinearColor::Green, FLinearColor::Red, DebugDrawTime);

	if (HitResult.GetActor()) {
		FString hitRes = HitResult.GetActor()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *hitRes);

		UGameplayStatics::ApplyDamage(HitResult.GetActor(), AttackDamage, AIController, OwnerPawn, UDamageType::StaticClass());
	}

	return EBTNodeResult::Succeeded;
}


