

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RepairObject.generated.h"

UCLASS()
class ZOMBIESHOOTER_API ARepairObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ARepairObject();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMesh* ObjectMesh;

	virtual void BeginPlay() override;


};
