// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefence/Framework/ObjectPool/ObjectPoolDataAsset.h"
#include "TowerDefenceGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, int32 /*InCurrentGold*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float /*InCurrentHealthRatio*/);
DECLARE_MULTICAST_DELEGATE(FOnGameClear);
DECLARE_MULTICAST_DELEGATE(FOnGameOver);

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
	// 적이 죽었을 때 호출(돈증가+남은적 수 감소)
	UFUNCTION()
	void OnEnemyKilled(int32 InGold);

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

	inline void SubtractHealth(int32 InValue) 
	{ 
		SetHealth(Health - InValue);
		if (bIsGamePlay && Health < 0)
		{
			Health = 0;
			GameOver();
		}

		DecreaseRemainingEnemyCount();
	}

	// 비어있는 타워빌더가 있는지 확인하는 함수(true면 비어있는 타워빌더가 있음)
	inline bool IsAnyEmptyTowerBuilder() const { return EmptyTowerBuilderCount > 0; }

private:
	void GameClear();
	void GameOver();

	inline void DecreaseRemainingEnemyCount()
	{
		RemainingEnemyCount--;
		if (bIsGamePlay && RemainingEnemyCount <= 0)
		{
			RemainingEnemyCount = 0;
			GameClear();
		}
	}

	inline void SetHealth(int32 InNewHealth)
	{
		Health = InNewHealth;
		OnHealthChanged.Broadcast(static_cast<float>(Health) / static_cast<float>(StartHealth)); // HealthBar에 현재 체력 비율 전달
	}

public:
	FOnGoldChanged OnGoldChanged;
	FOnHealthChanged OnHealthChanged;
	FOnGameClear OnGameClear;
	FOnGameOver OnGameOver;

protected:
	// 오브젝트 풀 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageData")
	UObjectPoolDataAsset* ObjectPoolDataAsset = nullptr; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageData")
	int32 Gold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageData")
	int32 Health = 100;

	// 비어있는 타워 빌더의 갯수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StageData")
	int32 EmptyTowerBuilderCount = 0;	

	// 이 스테이지에서 남아있는 적의 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StageData")
	int32 RemainingEnemyCount = 0;

private:
	bool bIsGamePlay = true;
	int32 StartHealth = 100;
};
