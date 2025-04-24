// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBuff_LongRange.h"

void UEffectBuff_LongRange::OnInitialize(EEffectType InType, const UEffectDataAsset* InData, AActor* InTarget)
{
	Super::OnInitialize(InType, InData, InTarget);
	EffectModifiers.Add(EEffectModifier::FireRange, EffectDataAsset->Modifier1);	// 사거리 증가
}
