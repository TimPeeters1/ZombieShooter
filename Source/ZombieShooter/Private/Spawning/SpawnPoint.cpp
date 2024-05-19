


#include "Spawning/SpawnPoint.h"

ASpawnPoint::ASpawnPoint()
{
	ArrowIndicator = CreateDefaultSubobject<UArrowComponent>("ArrowIndicator", false);
	ArrowIndicator->SetWorldRotation(FVector::UpVector.Rotation().Quaternion());
	ArrowIndicator->ArrowSize = 3;
}

FTransform ASpawnPoint::GetSpawnTransform()
{
	FVector SpawnLoc = ArrowIndicator->GetComponentLocation();
	FTransform SpawnPos = FTransform(SpawnLoc);
	return SpawnPos;
}




