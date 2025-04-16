// Fill out your copyright notice in the Description page of Project Settings.


#include "Debuff_Stun.h"
#include "TowerDefence/Enemy/EnemyBase.h"

UDebuff_Stun::UDebuff_Stun()
{
	DebuffName = FName("Stun");
	DebuffType = EDebuffType::Stun;
	DefaultDuration = 1.0f;
	CurrentDuration = 0.0f;
}

void UDebuff_Stun::OnInitialize(class AEnemyBase* InTarget, float InModifier)
{
	Super::OnInitialize(InTarget, InModifier);
	TargetEnemy->UpdateSpeedModifier();			// 스턴 상태에서는 이동 불가
	TargetEnemy->UpdateDamageModifier();		// 스턴 상태에서는 추가 데미지 적용
}

void UDebuff_Stun::OnEnd()
{
	TargetEnemy->UpdateDamageModifier(EDebuffType::Stun);	// 원래 데미지 배율로 되돌리기
	TargetEnemy->UpdateSpeedModifier(EDebuffType::Stun);	// 원래 속도로 되돌리기
	Super::OnEnd();
}
