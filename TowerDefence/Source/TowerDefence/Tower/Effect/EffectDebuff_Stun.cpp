// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDebuff_Stun.h"

void UEffectDebuff_Stun::UpdateModifiers()
{
	EffectModifiers.Add(EEffectModifier::MoveSpeed, -100.0f);	// 이동 속도를 무조건 0으로 하기 위해 -100.0f로 설정
	EffectModifiers.Add(EEffectModifier::Defence, GetModifier1());	// 방어력 감소
}
