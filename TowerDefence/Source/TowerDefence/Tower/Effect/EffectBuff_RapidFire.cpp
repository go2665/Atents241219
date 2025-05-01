// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBuff_RapidFire.h"

void UEffectBuff_RapidFire::UpdateModifiers()
{
	EffectModifiers.Add(EEffectModifier::FireRate, GetModifier1());	// 공격 속도 증가
}
