// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDamageType.h"
#include "IceDamageType.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UIceDamageType : public UTowerDamageType
{
	GENERATED_BODY()
	
public:
	static inline EDamageType GetAttributeType() { return EDamageType::Ice; } // 속성 타입을 반환하는 함수
};
