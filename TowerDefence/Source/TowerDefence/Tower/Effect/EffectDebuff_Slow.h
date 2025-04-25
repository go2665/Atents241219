// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBase.h"
#include "EffectDebuff_Slow.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UEffectDebuff_Slow : public UEffectBase
{
	GENERATED_BODY()

protected:
	virtual void UpdateModifiers() override;
};
