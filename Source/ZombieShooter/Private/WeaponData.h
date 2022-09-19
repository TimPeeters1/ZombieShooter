#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* WeaponModel; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform FP_Model_Transform;
};
