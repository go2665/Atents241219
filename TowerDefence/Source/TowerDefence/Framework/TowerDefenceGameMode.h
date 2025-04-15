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
	inline int GetGold() const { return Gold; }
	inline void SetGold(int NewGold) { Gold = NewGold; }
	inline void AddGold(int AddedGold) { Gold += AddedGold; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StageData")
	int Gold = 0;
};
