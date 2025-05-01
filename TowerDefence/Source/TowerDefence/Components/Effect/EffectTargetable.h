// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDefence/Tower/Data/EffectDataAsset.h"
#include "EffectTargetable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
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
	// 버프 추가 함수(이팩트 타입, 이팩트를 거는 대상의 레벨)
	virtual bool AddEffect(EEffectType InType, int32 InLevel) = 0;

	// 버프 제거 함수
	virtual bool RemoveEffect(EEffectType InType) = 0;

	// 타겟에 모디파이어를 적용하는 함수
	virtual void ApplyModifiers(const TMap<EEffectModifier, float>* InModifierMap ) = 0;

	// 모디파이어 맵을 가져오는 함수
	inline virtual const TMap<EEffectModifier, float>* GetModifiersMap() const = 0;

	// 특정 모디파이어를 가져오는 함수
	inline virtual float GetModifier(EEffectModifier ModifierType) const = 0;

	//// 타겟이 되는 액터의 체력을 설정하는 함수
	//inline virtual void SetHealth(float InHealth) = 0;

	//// 타겟이 되는 액터가 살아 있는지 체크하는 함수
	//inline virtual bool IsAlive() const = 0;

	// 필요한 맴버 변수(EffectComponent가 전달해 줄 것)
	// const TMap<EEffectModifier, float>* EffectModifiers = nullptr;
};
