// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Data/BuffData/TowerBuffDataAsset.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Defines/DebuffEnums.h"
#include "TowerDefence/Defines/DamageAttribute/TowerDamageType.h"
#include "DEPRECATED_SkillDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API U_DEPRECATED_SkillDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float Radius = 100.0f;	// 스킬 범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CooldownTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	UNiagaraSystem* SkillEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<UTowerDamageType> DamageType = nullptr;	// 데미지 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<UTowerBuffDataAsset*> TowerBuffDataAssets;	// 타워 버프 데이터 에셋

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<EDebuffType> DebuffTypes;	// 디버프 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float DebuffModifier = 1.0f;	// 디버프 배율
};
