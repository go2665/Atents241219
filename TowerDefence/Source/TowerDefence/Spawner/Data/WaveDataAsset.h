// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "WaveDataAsset.generated.h"

class AEnemy;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FEnemyGroupData
{
	GENERATED_BODY()

	// 풀에서의 적타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Group")
	EPooledActorType EnemyType = EPooledActorType::EnemySpeed; 

	// 스폰할 수량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Group")
	int32 SpawnCount = 1;

	// 스폰 인터벌(음수면 한번에 생성))
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Group")
	float SpawnInterval = 0.1f;
};

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()
	
	// 웨이브 시작 시간(초 단위)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float StartTime = 0.0f;

	// 적 그룹(적의 종류(BP), 스폰할 수량, 스폰 인터벌(음수면 한번에 생성))
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FEnemyGroupData> EnemyGroups;
};

UCLASS()
class TOWERDEFENCE_API UWaveDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	// 웨이브 이름
	FString WaveName; // 데이터 에셋 편집자 확인용(코드에서는 사용하지 않음)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FWaveData> WaveDatas;
	
};
