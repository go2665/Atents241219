// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "Action/Enemy/NormalEnemy.h"
#include "AIController.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANormalEnemy* Enemy = Cast<ANormalEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy)
	{
		Enemy->Attack();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
