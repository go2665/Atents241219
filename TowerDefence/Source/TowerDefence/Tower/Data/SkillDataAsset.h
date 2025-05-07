// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "TowerDefence/Defines/DamageAttribute/TowerDamageType.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "AoeDataAsset.h"
#include "SkillDataAsset.generated.h"


// 레벨별 데이터를 가지는 구조체
USTRUCT(BlueprintType)
struct FSkillLevelData
{
	GENERATED_BODY()

	// 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Level Data", meta = (ClampMin = "0"))
	float CooldownTime = 5.0f;

	// 반지름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Level Data", meta = (ClampMin = "0"))
	float Radius = 100.0f;
};

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API USkillDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FString SkillName;	
		
	// 스킬 이팩트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	UNiagaraSystem* SkillEffect = nullptr;

	// 스킬이 다른 타워에게 제공할 버프 종류(None일 경우, 타워에게 버프를 제공하지 않음)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EEffectType BuffType = EEffectType::None;

	// 스킬이 적에게 제공할 디버프 종류(None일 경우, 적에게 디버프를 제공하지 않음)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EEffectType DebuffType = EEffectType::None;

	// 스킬의 AOE 데이터(null이 아니면 AOE가 설치된다.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	UAoeDataAsset* AoeData = nullptr;

	// 레벨별 데이터(쿨타임, 반지름)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FSkillLevelData> LevelData;	

};
