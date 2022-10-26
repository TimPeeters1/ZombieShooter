
#include "BTTask_TravelToActiveArea.h"
#include "SpawnManager.h"
#include "Engine/TriggerBox.h"

#include "NavigationSystem.h"

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
		if (!GameMode->SpawnManager->ActivePlayerArea.IsEmpty()) {
			ASpawnArea* NewTravelArea = GameMode->SpawnManager->ActivePlayerArea[FMath::RandRange(0, GameMode->SpawnManager->ActivePlayerArea.Num() - 1)];

			FVector origin;
			FVector extents;
			NewTravelArea->GetActorBounds(false, origin, extents, false);

			FVector RandomPos = UKismetMathLibrary::RandomPointInBoundingBox(origin, extents);
			//FVector ProjectedPos = UNavigationSystemV1::GetRandomPointInNavigableRadius(GetWorld(), RandomPos, 300.0f);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), RandomPos);
			DrawDebugPoint(GetWorld(), RandomPos, 20.0f, FColor::Blue, false, 3.0f);

			return EBTNodeResult::Succeeded;
		}
		/*
			APawn* PawnRef = GameMode->PlayerCharacters[FMath::RandRange(0, GameMode->PlayerCharacters.Num() - 1)]->GetPawn();
			if (PawnRef) {

				return EBTNodeResult::Succeeded;

			} else return EBTNodeResult::Failed;
		*/

		return EBTNodeResult::Succeeded;

	}else return EBTNodeResult::Failed;

}
