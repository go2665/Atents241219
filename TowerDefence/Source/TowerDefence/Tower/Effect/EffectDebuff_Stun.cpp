// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDebuff_Stun.h"

void UEffectDebuff_Stun::UpdateModifiers()
{
	EffectModifiers.Add(EEffectModifier::MoveSpeed, -100.0f * StackCount);
}
