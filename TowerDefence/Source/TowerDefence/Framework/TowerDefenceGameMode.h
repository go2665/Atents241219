// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefenceGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, int32 /*InCurrentGold*/);

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API ATowerDefenceGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	inline int32 GetGold() const { return Gold; }
	inline void SetGold(int32 InNewGold) { Gold = InNewGold; OnGoldChanged.Broadcast(Gold); }
	inline void AddGold(int32 InAddedGold) { SetGold(Gold + InAddedGold); }
	inline bool UseGold(int32 InUsedGold) { 
		if (Gold >= InUsedGold)
		{
			SetGold(Gold - InUsedGold);
			return true;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Not enough gold!"));
		return false;
	}

	// 비어있는 타워빌더가 있는지 확인하는 함수(true면 비어있는 타워빌더가 있음)
	inline bool IsAnyEmptyTowerBuilder() const { return EmptyTowerBuilderCount > 0; }

public:
	FOnGoldChanged OnGoldChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageData")
	int32 Gold = 0;

	// 비어있는 타워 빌더의 갯수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StageData")
	int32 EmptyTowerBuilderCount = 0;	
};
