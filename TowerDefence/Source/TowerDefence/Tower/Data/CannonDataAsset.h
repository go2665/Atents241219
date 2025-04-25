// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "CannonDataAsset.generated.h"

/**
 * 타워 위에 올라가는 대포의 데이터
 */

class ACannon;

// 레벨별 데이터를 가지는 구조체
USTRUCT(BlueprintType)
struct FCannonLevelData
{
	GENERATED_BODY()
	
	// 시간 당 공격 회수(1초당 2발)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon|Level Data", meta = (ClampMin = "0.1"))
	float FireRate = 2.0f;

	// 공격 사정거리(cm 단위, 3m)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon|Level Data", meta = (ClampMin = "100.0"))
	float Range = 300.0f;

	// 공격 시 한번에 공격 가능한 타겟 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon|Level Data", meta = (ClampMin = "1"))
	int32 TargetCount = 1;

	// 업그레이드 비용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon|Level Data", meta = (ClampMin = "1"))
	int32 UpgradeCost = 50;

};

UCLASS()
class TOWERDEFENCE_API UCannonDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 대포의 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon")
	TSubclassOf<ACannon> CannonClass;

	// 대포의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon")
	FText GunName;

	// TargetCount가 여러명일 때 발사 순서(발견 순서 or 거리 순서)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon")
	EFireOrder FireOrder = EFireOrder::Discovery;	

	// 대포의 레벨별 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon")
	TArray<FCannonLevelData> LevelData;
};
