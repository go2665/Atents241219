// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Data/ShotData/_DEPRECATED_ShotDataAsset.h"
#include "GunDataAsset.generated.h"

/**
 * 타워 위에 올라가는 총기의 데이터
 */
UCLASS()
class TOWERDEFENCE_API UGunDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	FText GunName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	float FireRate = 2;			// 시간 당 공격(1초당 2발)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	float Range = 100.0f;		// cm 단위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	int32 TargetCount = 1;		// 공격 시 타겟 수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	int32 UpgradeCost = 50;		// 업그레이드 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	U_DEPRECATED_ShotDataAsset* ShotData;	// 발사체 데이터
};
