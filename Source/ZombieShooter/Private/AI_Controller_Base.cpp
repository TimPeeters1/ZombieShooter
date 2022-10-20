#include "AI_Controller_Base.h"
#include "ZombiePawn.h"

AAI_Controller_Base::AAI_Controller_Base()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");
}

void AAI_Controller_Base::BeginPlay()
{
	Super::BeginPlay();
	AZombiePawn* AIChar = Cast<AZombiePawn>(GetPawn());
	if(AIChar)
		TeamId = FGenericTeamId(1)
}

ETeamAttitude::Type AAI_Controller_Base::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (!OtherPawn) return ETeamAttitude::Neutral;

	auto PlayerTI = Cast<IGenericTeamAgentInterface>(&Other);
	class IGenericTeamAgentInterface* BotTI = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (!PlayerTI && !BotTI) {
		return ETeamAttitude::Neutral;
	}

	FGenericTeamId OtherTeamId = NULL;
	if (PlayerTI)
		OtherTeamId = PlayerTI->GetGenericTeamId();
	else if (BotTI)
		OtherTeamId = BotTI->GetGenericTeamId();


	FGenericTeamId ThisTeamId = GetGenericTeamId();
	if (OtherTeamId == ThisTeamId)
		return ETeamAttitude::Friendly;
	else 		
		return ETeamAttitude::Hostile;
}


