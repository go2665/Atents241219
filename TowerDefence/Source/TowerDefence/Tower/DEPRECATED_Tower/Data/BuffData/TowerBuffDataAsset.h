// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Defines/TowerBuffEnums.h"
#include "TowerBuffDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerBuffDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Buff Data")
	FText BuffName;	// 버프 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Buff Data")
	ETowerBuffType BuffType = ETowerBuffType::None;	// 버프 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Buff Data")
	float Duration = 1.0f; // 디버프 기본 지속 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Buff Data")
	float Modifier1 = 0.0f;	// 버프 수치 1

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Buff Data")
	float Modifier2 = 0.0f;	// 버프 수치 2

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower Buff Data")
	float Modifier3 = 0.0f;	// 버프 수치 3
};
