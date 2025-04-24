// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBuff_PowerUp.h"

void UEffectBuff_PowerUp::OnInitialize(EEffectType InType, const UEffectDataAsset* InData, AActor* InTarget)
{
	Super::OnInitialize(InType, InData, InTarget);
	EffectModifiers.Add(EEffectModifier::FireDamage, EffectDataAsset->Modifier1);	// 공격력 증가
}
