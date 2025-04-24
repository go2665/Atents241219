// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDebuff_Slow.h"

void UEffectDebuff_Slow::OnInitialize(EEffectType InType, const UEffectDataAsset* InData, AActor* InTarget)
{
	Super::OnInitialize(InType, InData, InTarget);
	EffectModifiers.Add(EEffectModifier::MoveSpeed, EffectDataAsset->Modifier1);	// 이동 속도 감소
}
