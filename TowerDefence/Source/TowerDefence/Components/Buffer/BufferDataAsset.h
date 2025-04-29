// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "BufferDataAsset.generated.h"

// 레벨별 데이터를 가지는 구조체
USTRUCT(BlueprintType)
struct FBufferLevelData
{
	GENERATED_BODY()
		
	// 인터벌 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffer|Level Data", meta = (ClampMin = "0"))
	float Interval = 1.0f;

	// 반지름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffer|Level Data", meta = (ClampMin = "0"))
	float Radius = 100.0f;
};

/**
 * 버퍼 컴포넌트에서 사용할 데이터
 */
UCLASS(BlueprintType)
class TOWERDEFENCE_API UBufferDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 버프 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffer")
	EEffectType EffectType = EEffectType::None;

	// 레벨별 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buffer")
	TArray<FBufferLevelData> LevelData;
};
