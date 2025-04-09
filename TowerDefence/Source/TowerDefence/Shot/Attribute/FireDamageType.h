// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "TowerDefence/Shot/Data/ShotEnums.h"
#include "TowerDamageType.h"
#include "FireDamageType.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UFireDamageType : public UTowerDamageType
{
	GENERATED_BODY()
	
public:
	static inline EAttributeType GetAttributeType() { return EAttributeType::Fire; } // 속성 타입을 반환하는 함수
};
