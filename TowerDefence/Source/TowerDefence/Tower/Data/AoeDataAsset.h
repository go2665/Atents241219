// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "TowerDefence/Defines/DamageAttribute/TowerDamageType.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDefence/Tower/Skill/AreaOfEffect.h"
#include "AoeDataAsset.generated.h"


// 레벨별 데이터를 가지는 구조체
USTRUCT(BlueprintType)
struct FAoeLevelData
{
	GENERATED_BODY()

	// 데미지(즉시)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Level Data", meta = (ClampMin = "0"))
	float Damage = 0.0f;	

	// 데미지(지속, 틱당 데미지)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Level Data", meta = (ClampMin = "0"))
	float DotDamage = 0.0f;

	// 지속 데미지 인터벌
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Level Data", meta = (ClampMin = "0"))
	float DotInterval = 1.0f;	// 틱당 데미지 주는 시간 간격

	// 전체 지속시간(AOE가 남아있는 시간)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Level Data", meta = (ClampMin = "0"))
	float Duration = 5.0f;	
};

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UAoeDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	TSubclassOf<AAreaOfEffect> AoeClass = nullptr;

	// 데미지 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	TSubclassOf<UTowerDamageType> DamageType = nullptr;

	// 레벨별 데이터(쿨타임, 반지름)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	TArray<FAoeLevelData> LevelData;
};
