// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDamageType.h"
#include "LightningDamageType.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API ULightningDamageType : public UTowerDamageType
{
	GENERATED_BODY()
	
public:
	static inline EDamageType GetAttributeType() { return EDamageType::Lightning; } // 속성 타입을 반환하는 함수
};
