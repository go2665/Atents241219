// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBuff_PowerUp.h"

void UEffectBuff_PowerUp::UpdateModifiers()
{
	EffectModifiers.Add(EEffectModifier::Attack, GetModifier1());	// 공격력 증가
}
