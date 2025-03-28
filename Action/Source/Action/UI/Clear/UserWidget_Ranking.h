// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Action/Framework/SaveGame_Rank.h"
#include "Action/UI/Clear/UserWidget_RankLine.h"
#include "UserWidget_Ranking.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UUserWidget_Ranking : public UUserWidget
{
	GENERATED_BODY()

public:
	void RefreshRankList();

	inline void OnNewRanker(int32 InRank){ 
		if (InRank > RankLines.Num() * 0.5f)
		{
			RankList->ScrollToEnd();
		}
		RankLines[InRank]->OnNewRanker(); 
	}

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank", meta = (BindWidget))
	UScrollBox* RankList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank", meta = (BindWidget))
	TArray<UUserWidget_RankLine*> RankLines;
};
