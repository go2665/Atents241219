// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Action/Framework/SaveGame_Rank.h"
#include "UserWidget_Ranking.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UUserWidget_Ranking : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	void InitializeRankList(const TArray<FRankData>& Rank);

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank", meta = (BindWidget))
	class UScrollBox* RankList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank", meta = (BindWidget))
	TArray<class UUserWidget_RankLine*> RankLines;
};
