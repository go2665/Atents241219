// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/Defines/DamageAttribute/TowerDamageType.h"
#include "EnemyDataAsset.generated.h"

/**
 * 적의 데이터
 */

UCLASS()
class TOWERDEFENCE_API UEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 적의 기본 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Data")
	float Health = 20.0f;

	// 적의 기본 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Data")
	float Speed = 100.0f;

	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Data")
	float Defence = 0.0f;

	// 적의 약점 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Data")
	TSubclassOf<UTowerDamageType> WeakType = nullptr;

	// 골드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Data")
	int32 Gold = 10;

	// 적이 골인 지점에 도착했을 때 플레이어에게 주는 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Data")
	int32 Damage = 1;
};
