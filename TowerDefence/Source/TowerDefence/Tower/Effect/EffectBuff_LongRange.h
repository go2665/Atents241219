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

protected:
	virtual void UpdateModifiers() override;
};
