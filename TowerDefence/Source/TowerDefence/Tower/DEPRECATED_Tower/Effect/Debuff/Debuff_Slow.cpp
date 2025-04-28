// Fill out your copyright notice in the Description page of Project Settings.


#include "Debuff_Slow.h"
#include "TowerDefence/Enemy/DEPRECATED_Enemy/EnemyBase.h"

UDebuff_Slow::UDebuff_Slow()
{
	DebuffName = FName("Slow");
	DebuffType = EDebuffType::Slow;
	DefaultDuration = 3.0f; 
	CurrentDuration = 0.0f;
}

void UDebuff_Slow::OnInitialize(class AEnemyBase* InTarget, float InModifier)
{
	Super::OnInitialize(InTarget, InModifier);	
	TargetEnemy->UpdateSpeedModifier();
}

void UDebuff_Slow::OnEnd()
{
	TargetEnemy->UpdateSpeedModifier(EDebuffType::Slow); // 원래 속도로 되돌리기
	Super::OnEnd();
}
