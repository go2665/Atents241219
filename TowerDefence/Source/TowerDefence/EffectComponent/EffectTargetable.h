// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "EffectTargetable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEffectTargetable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOWERDEFENCE_API IEffectTargetable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyModifiers(const TMap<EEffectModifier, float>& InModifierMap ) = 0;

	// 타겟이 되는 액터의 체력을 설정하는 함수
	virtual void SetHealth(float InHealth) = 0;

	// 타겟이 되는 액터가 살아 있는지 체크하는 함수
	virtual bool IsAlive() const = 0;
};
