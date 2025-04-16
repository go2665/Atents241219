// Fill out your copyright notice in the Description page of Project Settings.


#include "DebuffBase.h"
#include "TowerDefence/Enemy/EnemyBase.h"

UDebuffBase::UDebuffBase()
{
	DebuffName = FName("Base");
}

void UDebuffBase::OnInitialize(AEnemyBase* InTarget, float InModifier)
{
	TargetEnemy = InTarget;
	ModifierValue = InModifier;
	CurrentDuration = DefaultDuration;

	// 이 디버프가 처음 적용된 적의 상태를 변경하는 로직을 여기에 추가합니다.
}

void UDebuffBase::OnTick(float DeltaTime)
{
	CurrentDuration -= DeltaTime;
}

void UDebuffBase::OnEnd()
{
	// 이 디버프가 끝났을 때 적의 상태를 원래대로 되돌리는 로직을 여기에 추가합니다.
	TargetEnemy = nullptr;
	CurrentDuration = 0.0f;
}
