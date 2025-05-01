// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDebuff_Slow.h"

void UEffectDebuff_Slow::UpdateModifiers()
{
	EffectModifiers.Add(EEffectModifier::MoveSpeed, GetModifier1());	// 이동 속도 감소
}
