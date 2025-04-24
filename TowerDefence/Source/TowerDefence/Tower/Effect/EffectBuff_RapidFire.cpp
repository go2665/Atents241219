// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectBuff_RapidFire.h"

void UEffectBuff_RapidFire::OnInitialize(EEffectType InType, const UEffectDataAsset* InData, AActor* InTarget)
{
	Super::OnInitialize(InType, InData, InTarget);
	EffectModifiers.Add(EEffectModifier::FireRate, EffectDataAsset->Modifier1);	// 공격 속도 증가
}
