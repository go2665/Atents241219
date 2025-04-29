// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TowerDataAsset.generated.h"

/**
 * 타워의 데이터
 */

class ATower;

UCLASS()
class TOWERDEFENCE_API UTowerDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 타워의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	FText TowerName;

	// 타워의 액터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	TSubclassOf<ATower> TowerClass = nullptr;

	// 타워의 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	int32 TowerCost = 100;

	// 타워의 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	UTexture* TowerImage = nullptr;
};
