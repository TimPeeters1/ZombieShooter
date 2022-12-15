// Project 'Zombie Shooter' -Tim Peeters 2022/23

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "RepairData.generated.h"

class UMetaSoundSource;

UENUM(BlueprintType)
enum class ERepairObjectType : uint8 {
	UNDEFINED = 0		UMETA(DisplayName = "UNDEFINED"),
	FUEL = 1			UMETA(DisplayName = "Fuel"),
	ENGINE_PARTS = 2    UMETA(DisplayName = "EngineParts"),
	TIRES = 3			UMETA(DisplayName = "Tires"),
	CAR_BATTERY = 4     UMETA(DisplayName = "CarBattery"),
	CAR_KEYS = 5		UMETA(DisplayName = "CarKeys")
};

UCLASS(Blueprintable)
class ZOMBIESHOOTER_API URepairData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ObjectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ERepairObjectType ObjectType;

	UPROPERTY(Category = "Visuals|Mesh", EditAnywhere, BlueprintReadWrite)
		UStaticMesh* ObjectMesh;

	UPROPERTY(Category = "Effects|Mesh", EditAnywhere, BlueprintReadWrite)
		FVector ObjectScale = FVector::OneVector;

	UPROPERTY(Category = "Effects|Audio", EditAnywhere, BlueprintReadWrite)
		UMetaSoundSource* RepairAudio;

};
