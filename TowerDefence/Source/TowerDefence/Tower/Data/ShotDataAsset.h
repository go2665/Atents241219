// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/DEPRECATED_Tower/Defines/ShotEnums.h"
#include "TowerDefence/DEPRECATED_Tower/Defines/DamageAttribute/TowerDamageType.h"
#include "TowerDefence/DEPRECATED_Tower/Defines/DebuffEnums.h"
#include "ShotDataAsset.generated.h"

class AProjectile;

// 레벨별 데이터를 가지는 구조체
USTRUCT(BlueprintType)
struct FShotLevelData
{
	GENERATED_BODY()

	// 한발 당 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Level Data")
	float Damage = 1;

	// 디버프 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Level Data")
	EDebuffType DebuffType = EDebuffType::None;

	// 범위 공격 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Splash")
	float SplashRadius = 100.0f;
};

/**
 * 총이 발사하는 총알의 데이터
 */
UCLASS()
class TOWERDEFENCE_API UShotDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 속성 타입(데미지 타입)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot")
	TSubclassOf<UTowerDamageType> AttributeType = UTowerDamageType::StaticClass(); // 속성 타입(데미지 타입)
	
	// 레벨별 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot")
	TArray<FShotLevelData> LevelData;

	// 발사체 클래스(없으면 히트스캔)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Shot")
	TSubclassOf<AProjectile> ProjectileClass = nullptr;
	
	// 단일/범위 공격 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Splash")
	bool bIsSplashAttack = false;

	// 범위 공격 감쇠 비율(0.0 ~ 1.0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Splash")
	float SplashFalloff = 0.8f;
};
