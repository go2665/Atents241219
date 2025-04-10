// Fill out your copyright notice in the Description page of Project Settings.


#include "Debuff_Stun.h"

UDebuff_Stun::UDebuff_Stun()
{
	DebuffName = FName("Stun");
	DebuffType = EDebuffType::Stun;
	DefaultDuration = 1.0f;
	CurrentDuration = 0.0f;
}

void UDebuff_Stun::OnInitialize(AEnemyBase* Target)
{
	Super::OnInitialize(Target);
	TargetEnemy->SetSpeedModifier(StunSpeedModifier); // 스턴 상태에서는 이동 불가
	TargetEnemy->SetDamageModifier(StunDamageModifier); // 스턴 상태에서는 추가 데미지 적용
}

void UDebuff_Stun::OnEnd()
{
	TargetEnemy->SetDamageModifier(1.0f); // 원래 데미지 배율로 되돌리기
	TargetEnemy->SetSpeedModifier(1.0f);	// 원래 속도로 되돌리기
	Super::OnEnd();
}
