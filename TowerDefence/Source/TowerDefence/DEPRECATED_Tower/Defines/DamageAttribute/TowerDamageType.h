// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "TowerDefence/DEPRECATED_Tower/Defines/ShotEnums.h"
#include "TowerDamageType.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	static inline EAttributeType GetAttributeType() { return EAttributeType::None; } // 속성 타입을 반환하는 함수

};
