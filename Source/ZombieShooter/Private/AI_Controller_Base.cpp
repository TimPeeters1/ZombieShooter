#include "AI_Controller_Base.h"
#include "ZombiePawn.h"
#include "PlayerPawn.h"

#include "Perception/AISenseConfig_Sight.h"

AAI_Controller_Base::AAI_Controller_Base(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTConstruct(TEXT("BehaviorTree'/Game/_GAME/Blueprints/Gameplay/Characters/AI/BT_Zombie.BT_Zombie'"));
	if (BTConstruct.Succeeded())
	{
		BehaviorTree = BTConstruct.Object;
	}

	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	AIPerceptionComponent = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerceptionComponent"));

	SightPercpetion = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("SightConfig"));

	SightPercpetion->SightRadius = 3000.0f;
	SightPercpetion->LoseSightRadius = 3500.0f;
	SightPercpetion->PeripheralVisionAngleDegrees = 105.f;
	SightPercpetion->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*SightPercpetion);
	AIPerceptionComponent->SetDominantSense(SightPercpetion->GetSenseImplementation());
}

void AAI_Controller_Base::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);

	if (BlackboardComponent)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

void AAI_Controller_Base::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controller_Base::OnPerception);
}


void AAI_Controller_Base::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	TArray<AActor*> DetectedThreats;
	AIPerceptionComponent->GetHostileActors(DetectedThreats);
	if (DetectedThreats.IsEmpty()) return;
	if (!DetectedThreats.Contains(Actor)) return;

	//Set Focus on Actual First Sensed Actor. Only changes when lost sight.
	SetFocus(Stimulus.WasSuccessfullySensed() ? Actor : nullptr);

	if (!BlackboardComponent) return;

	if (GetFocusActor()) {
		BlackboardComponent->SetValueAsObject(FName("TargetActor"), GetFocusActor());

		//Update Last Know Location in BB
		BlackboardComponent->SetValueAsVector(FName("TargetLocation"), GetFocusActor()->GetActorLocation());
	}
	else {
		BlackboardComponent->SetValueAsObject(FName("TargetActor"), nullptr);
	}

}

ETeamAttitude::Type AAI_Controller_Base::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (Cast<AZombiePawn>(&Other)) {
		return ETeamAttitude::Friendly;
	}
	else if (Cast<APlayerPawn>(&Other)) {
		return ETeamAttitude::Hostile;
	}
	else {
		return ETeamAttitude::Neutral;
	}

}

