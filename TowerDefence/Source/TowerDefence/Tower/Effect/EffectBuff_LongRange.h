// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBase.h"
#include "EffectBuff_LongRange.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UEffectBuff_LongRange : public UEffectBase
{
	GENERATED_BODY()

public:
	virtual void OnInitialize(EEffectType InType, const UEffectDataAsset* InData, AActor* InTarget) override;
};
