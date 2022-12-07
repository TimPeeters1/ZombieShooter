
#include "BTTask_TravelToActiveArea.h"
#include "Engine/TriggerBox.h"
#include "NavigationSystem.h"

#include "GameInstance_Main.h"
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

	UGameInstance_Main* GameInstance = (UGameInstance_Main*)UGameplayStatics::GetGameInstance(GetWorld());
	if (GameInstance) {
		if (!GameInstance->SpawnManager) return EBTNodeResult::Failed;

		if (!GameInstance->SpawnManager->ActivePlayerArea.IsEmpty()) {
			ASpawnArea* NewTravelArea = GameInstance->SpawnManager->ActivePlayerArea[FMath::RandRange(0, GameInstance->SpawnManager->ActivePlayerArea.Num() - 1)];

			FVector origin;
			FVector extents;
			NewTravelArea->GetActorBounds(false, origin, extents, false);

			FVector RandomPos = UKismetMathLibrary::RandomPointInBoundingBox(origin, extents);
			//FVector ProjectedPos = UNavigationSystemV1::GetRandomPointInNavigableRadius(GetWorld(), RandomPos, 300.0f);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), RandomPos);
			//DrawDebugPoint(GetWorld(), RandomPos, 20.0f, FColor::Blue, false, 3.0f);

			return EBTNodeResult::Succeeded;
		}
		/*
			APawn* PawnRef = GameMode->	GameInstance->PlayerControllers[FMath::RandRange(0, GameMode->	GameInstance->PlayerControllers.Num() - 1)]->GetPawn();
			if (PawnRef) {

				return EBTNodeResult::Succeeded;

			} else return EBTNodeResult::Failed;
		*/

		return EBTNodeResult::Succeeded;

	}else return EBTNodeResult::Failed;

}
