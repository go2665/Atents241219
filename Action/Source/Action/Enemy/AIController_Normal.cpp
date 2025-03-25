// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Normal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


void AAIController_Normal::BeginPlay()
{
	Super::BeginPlay();
	
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (BlackboardComp)
	{
		UWorld* World = GetWorld();
		APlayerController* PlayerController = World->GetFirstPlayerController();
		APawn* PlayerPawn = PlayerController->GetPawn();
		BlackboardComp->SetValueAsObject("TargetPlayer", PlayerPawn);
	}

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}
