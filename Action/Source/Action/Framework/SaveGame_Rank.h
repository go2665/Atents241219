// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGame_Rank.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FRankData
{
	GENERATED_BODY()

public:
	FRankData()
		: RankName(FText::GetEmpty()), Gold(0)
	{
	}

	FRankData(const FText& InRankName, int32 InGold)
		: RankName(InRankName), Gold(InGold)
	{
	}

	UPROPERTY(BlueprintReadWrite)
	FText RankName;

	UPROPERTY(BlueprintReadWrite)
	int32 Gold;
};

UCLASS()
class ACTION_API USaveGame_Rank : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	TArray<FRankData> RankDataArray;
};
