// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "Action/Enemy/NormalEnemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANormalEnemy* Enemy = OwnerComp.GetOwner<ANormalEnemy>();
	if (Enemy)
	{
		Enemy->Attack();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
