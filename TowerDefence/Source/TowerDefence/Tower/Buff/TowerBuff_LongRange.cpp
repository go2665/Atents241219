// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuff_LongRange.h"

void UTowerBuff_LongRange::OnInitialize(UTowerBuffDataAsset* Data)
{
	BuffModifiers.Add(ETowerBuffModifier::Range, 1 + BuffDataAsset->Modifier1);	// 사거리 증가
}
