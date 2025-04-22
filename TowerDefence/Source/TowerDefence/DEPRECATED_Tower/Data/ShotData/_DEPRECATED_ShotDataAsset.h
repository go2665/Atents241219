// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/DEPRECATED_Tower/Defines/ShotEnums.h"
#include "TowerDefence/DEPRECATED_Tower/Defines/DamageAttribute/TowerDamageType.h"
#include "TowerDefence/DEPRECATED_Tower/Defines/DebuffEnums.h"
#include "_DEPRECATED_ShotDataAsset.generated.h"

/**
 * 총이 발사하는 총알의 데이터
 */
UCLASS()
class TOWERDEFENCE_API U_DEPRECATED_ShotDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	float Damage = 1;			// 한발 당 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	EDebuffType DebuffType = EDebuffType::None; // 디버프 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	bool bIsAreaAttack = false;					// 단일/범위 공격 여부
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	float AreaRadius = 100.0f;					// 범위 공격 반경

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	float AreaFalloff = 0.5f;					// 범위 공격 감쇠 비율(0.0 ~ 1.0)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	//EAttributeType AttributeType = EAttributeType::None; // 속성 타입
    TSubclassOf<UTowerDamageType> AttributeType = UTowerDamageType::StaticClass(); // 속성 타입(데미지 타입)

};
