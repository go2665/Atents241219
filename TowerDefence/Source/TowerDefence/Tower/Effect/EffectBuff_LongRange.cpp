// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBuff_LongRange.h"

void UEffectBuff_LongRange::UpdateModifiers()
{
	EffectModifiers.Add(EEffectModifier::FireRange, EffectDataAsset->Modifier1 * StackCount);	// 사거리 증가
}
