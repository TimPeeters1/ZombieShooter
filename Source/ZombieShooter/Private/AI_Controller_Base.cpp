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

	SightPercpetion->SightRadius = 2000.0f;
	SightPercpetion->LoseSightRadius = 2400.0f;
	SightPercpetion->PeripheralVisionAngleDegrees = 90.f;
	SightPercpetion->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*SightPercpetion);
	AIPerceptionComponent->SetDominantSense(SightPercpetion->GetSenseImplementation());
}

void AAI_Controller_Base::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);

	
	ZombieStateChanged.Broadcast();

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
	//SetFocus(Stimulus.WasSuccessfullySensed() ? Actor : nullptr);

	if (!BlackboardComponent) return;

	if (Actor) {
		Current_AIState = E_AI_State::FOLLOWING;
		BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);

		//Update Last Know Location in BB
		BlackboardComponent->SetValueAsVector(FName("TargetLocation"), Actor->GetActorLocation());

		/** Call an event dispatcher to notify observers. */

	}
	else {
		Current_AIState = E_AI_State::IDLE;
		BlackboardComponent->SetValueAsObject(FName("TargetActor"), nullptr);
	}

	BlackboardComponent->SetValueAsEnum(FName("AI_State"), (uint8)Current_AIState);
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


