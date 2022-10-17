

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

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

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackSettings", meta = (AllowPrivateAcces = "true"))
		float AttackSphereSize = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DebugSettings", meta = (AllowPrivateAcces = "true"))
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DebugSettings", meta = (AllowPrivateAcces = "true"))
		float DebugDrawTime = 5.0f;
};


