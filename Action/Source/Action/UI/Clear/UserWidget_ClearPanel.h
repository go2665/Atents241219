// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Action/UI/Clear/UserWidget_Ranking.h"
#include "UserWidget_ClearPanel.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UUserWidget_ClearPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	inline void RefreshRankList() { if (RankingPanel) RankingPanel->RefreshRankList(); };
	inline void OnNewRanker(int32 InRank) { if (RankingPanel) RankingPanel->OnNewRanker(InRank); };
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clear", meta = (BindWidget))
	UUserWidget_Ranking* RankingPanel;
};
