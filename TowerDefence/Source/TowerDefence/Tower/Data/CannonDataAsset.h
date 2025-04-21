// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CannonDataAsset.generated.h"

/**
 * 타워 위에 올라가는 대포의 데이터
 */

class ACannonBase;

UCLASS()
class TOWERDEFENCE_API UCannonDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 대포의 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon Data")
	TSubclassOf<ACannonBase> CannonClass;

	// 대포의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon Data")
	FText GunName;

	// 시간 당 공격 회수(1초당 2발)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon Data")
	float FireRate = 2.0f;			

	// 공격 사정거리(cm 단위, 3m)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon Data")
	float Range = 300.0f;		

	// 공격 시 한번에 공격 가능한 타겟 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon Data")
	int32 TargetCount = 1;		

	// 업그레이드 비용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Cannon Data")
	int32 UpgradeCost = 50;		

};
