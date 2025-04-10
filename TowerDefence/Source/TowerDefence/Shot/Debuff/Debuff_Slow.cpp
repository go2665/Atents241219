// Fill out your copyright notice in the Description page of Project Settings.


#include "Debuff_Slow.h"

UDebuff_Slow::UDebuff_Slow()
{
	DebuffName = FName("Slow");
	DebuffType = EDebuffType::Slow;
	DefaultDuration = 3.0f; 
	CurrentDuration = 0.0f;
}

void UDebuff_Slow::OnInitialize(AEnemyBase* Target)
{
	Super::OnInitialize(Target);	
	TargetEnemy->SetSpeedModifier(SlowRate);
}

void UDebuff_Slow::OnEnd()
{
	TargetEnemy->SetSpeedModifier(1.0f); // 원래 속도로 되돌리기
	Super::OnEnd();
}
