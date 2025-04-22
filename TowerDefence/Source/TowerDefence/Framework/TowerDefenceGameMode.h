// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefenceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API ATowerDefenceGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	inline int32 GetGold() const { return Gold; }
	inline void SetGold(int32 NewGold) { Gold = NewGold; }
	inline void AddGold(int32 AddedGold) { Gold += AddedGold; }
	inline bool UseGold(int32 UsedGold) { 
		if (Gold > UsedGold)
		{
			Gold -= UsedGold; 
			return true;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Not enough gold!"));
		return false;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageData")
	int32 Gold = 0;
};
