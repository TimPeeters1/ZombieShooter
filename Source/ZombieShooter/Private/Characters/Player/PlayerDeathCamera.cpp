#include "Characters/Player/PlayerDeathCamera.h"


APlayerDeathCamera::APlayerDeathCamera()
{
	GetCameraComponent()->bConstrainAspectRatio = false;
	GetCameraComponent()->SetFieldOfView(90.0f);
}

void APlayerDeathCamera::SetCameraPosition(FVector CameraLookAtPos)
{
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CameraLookAtPos));

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this, 3.0f);
}



