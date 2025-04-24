// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBuff_Hero.h"

void UEffectBuff_Hero::OnInitialize(EEffectType InType, const UEffectDataAsset* InData, AActor* InTarget)
{
	Super::OnInitialize(InType, InData, InTarget);
	EffectModifiers.Add(EEffectModifier::FireDamage, EffectDataAsset->Modifier1);	// 공격력 증가
	EffectModifiers.Add(EEffectModifier::FireRate, EffectDataAsset->Modifier2);		// 공격 속도 증가
	EffectModifiers.Add(EEffectModifier::FireRange, EffectDataAsset->Modifier3);	// 사거리 증가
}
