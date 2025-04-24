// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDebuff_Stun.h"

void UEffectDebuff_Stun::OnInitialize(EEffectType InType, const UEffectDataAsset* InData, AActor* InTarget)
{
	Super::OnInitialize(InType, InData, InTarget);
	EffectModifiers.Add(EEffectModifier::MoveSpeed, -100.0f);
}
