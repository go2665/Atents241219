// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TowerBuffBase.h"
#include "TowerBuff_RapidFire.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerBuff_RapidFire : public UTowerBuffBase
{
	GENERATED_BODY()
	
public:
	virtual void OnInitialize(const ATowerBaseActor* InTargetTower, const UTowerBuffDataAsset* InData) override;
};
