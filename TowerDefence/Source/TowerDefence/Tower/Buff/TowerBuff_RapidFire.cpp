// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuff_RapidFire.h"

void UTowerBuff_RapidFire::OnInitialize(const ATowerBaseActor* InTargetTower, const UTowerBuffDataAsset* InData)
{
	Super::OnInitialize(InTargetTower, InData);
	BuffModifiers.Add(ETowerBuffModifier::FireRate, 1 + BuffDataAsset->Modifier1);	// 발사 속도 증가
}
