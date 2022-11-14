#include "PlayerDeathCamera.h"

APlayerDeathCamera::APlayerDeathCamera()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision Sphere");
	CollisionComponent->SetSphereRadius(15.0f, false);
	CollisionComponent->SetupAttachment(RootComponent);

	GetCameraComponent()->bConstrainAspectRatio = false;
	GetCameraComponent()->SetFieldOfView(90.0f);
}

void APlayerDeathCamera::BeginPlay()
{
	if (PlayerRef) {
		this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerRef->GetActorLocation()));
		GetCameraComponent()->SetFieldOfView(PlayerRef->GetFP_Camera()->FieldOfView);
	}
	else {
		GetCameraComponent()->SetFieldOfView(90.0f);
	}



	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this, 1.0f);
}


