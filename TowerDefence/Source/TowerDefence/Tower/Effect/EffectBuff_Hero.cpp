// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBuff_Hero.h"

void UEffectBuff_Hero::UpdateModifiers()
{
	EffectModifiers.Add(EEffectModifier::Attack, GetModifier1());		// 공격력 증가
	EffectModifiers.Add(EEffectModifier::FireRate, GetModifier2());		// 공격 속도 증가
	EffectModifiers.Add(EEffectModifier::FireRange, GetModifier3());	// 사거리 증가
}
