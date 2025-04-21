// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuff_PowerUp.h"

void UTowerBuff_PowerUp::OnInitialize(const ATowerBaseActor* InTargetTower, const UTowerBuffDataAsset* InData)
{
	Super::OnInitialize(InTargetTower, InData);
	BuffModifiers.Add(ETowerBuffModifier::Damage, 1 + BuffDataAsset->Modifier1);	// 공격력 증가
}
