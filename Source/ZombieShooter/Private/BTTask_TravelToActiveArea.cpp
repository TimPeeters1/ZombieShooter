
#include "BTTask_TravelToActiveArea.h"
#include "SpawnManager.h"

UBTTask_TravelToActiveArea::UBTTask_TravelToActiveArea()
{
	bNotifyTick = false;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
	NodeName = TEXT("Travel To Active Area");
}

EBTNodeResult::Type UBTTask_TravelToActiveArea::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//TODO Fix TEMP Implementation of UBTTask_TravelToActiveArea

	if (!UKismetSystemLibrary::IsServer(GetWorld()))
		return EBTNodeResult::Aborted;

	AGameMode_Main* GameMode = Cast<AGameMode_Main>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode) {
		ASpawnArea* NewTravelArea = GameMode->SpawnManager->ActivePlayerArea[FMath::RandRange(0, GameMode->SpawnManager->ActivePlayerArea.Num() - 1)];
		//UKismetMathLibrary::RandomPointInBoundingBox(NewTravelArea->GetActorLocation(), NewTravelArea->GetActorBounds(NewTravelArea->GetActorLocation(), NewTravelArea->GetActorScale());



			/*
			APawn* PawnRef = GameMode->PlayerCharacters[FMath::RandRange(0, GameMode->PlayerCharacters.Num() - 1)]->GetPawn();
			if (PawnRef) {
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), PawnRef->GetActorLocation());

				return EBTNodeResult::Succeeded;

			} else return EBTNodeResult::Failed;
			*/

	}else return EBTNodeResult::Failed;


}
