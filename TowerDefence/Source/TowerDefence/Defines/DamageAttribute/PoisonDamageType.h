// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDamageType.h"
#include "PoisonDamageType.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UPoisonDamageType : public UTowerDamageType
{
	GENERATED_BODY()
	
public:
	static inline EDamageType GetAttributeType() { return EDamageType::Poison; } // 속성 타입을 반환하는 함수

};
