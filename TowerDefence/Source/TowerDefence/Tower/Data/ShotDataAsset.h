// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/Tower/Projectile.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDefence/Defines/DamageAttribute/TowerDamageType.h"
#include "ShotDataAsset.generated.h"

// 레벨별 데이터를 가지는 구조체
USTRUCT(BlueprintType)
struct FShotLevelData
{
	GENERATED_BODY()

	// 한발 당 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Level Data", meta = (ClampMin = "0"))
	float Damage = 1;

	// 디버프 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Level Data")
	EEffectType EffectType = EEffectType::None;

	// 범위 공격 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Splash", meta = (ClampMin = "10"))
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
	// 발사체인지 히트스캔인지 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Shot")
	EShootType ShootType = EShootType::HitScan;	

	// 발사체 종류(풀에서의 타입, ShootType이 Projectile일 때만 설정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Shot")
	EPooledActorType ProjectileType = EPooledActorType::ProjectileArrow;

	// 속성 타입(데미지 타입)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot")
	TSubclassOf<UTowerDamageType> DamageType = UTowerDamageType::StaticClass(); // 속성 타입(데미지 타입)
	
	// 레벨별 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot")
	TArray<FShotLevelData> LevelData;	
		
	// 단일/범위 공격 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Splash")
	bool bIsSplashAttack = false;

	// 범위 공격 감쇠 비율(0.0 ~ 1.0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot|Splash")
	float SplashFalloff = 0.8f;

	// 이 Shot이 Projectile인지 확인하는 함수(true면 발사체)
	UFUNCTION()
	inline bool IsProjectile() const { return ShootType == EShootType::Projectile; }

	// 이 Shot이 HitScan인지 확인하는 함수(true면 히트스캔)
	UFUNCTION()
	inline bool IsHitScan() const { return ShootType == EShootType::HitScan; }

	// 안전한 레벨 구하는 함수
	inline int32 GetSafeLevel(int32 InLevel) const
	{
		return FMath::Clamp(InLevel, 0, LevelData.Num() - 1);
	}

	// 레벨별 데이터 가져오는 함수
	inline const FShotLevelData& GetLevelData(int32 InLevel) const
	{
		return LevelData[GetSafeLevel(InLevel)];
	}
};
