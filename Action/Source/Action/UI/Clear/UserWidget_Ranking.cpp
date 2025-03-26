// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_Ranking.h"
#include "Components/ScrollBox.h"
#include "UserWidget_RankLine.h"
#include "Action/Framework/ActionGameMode.h"

void UUserWidget_Ranking::NativeConstruct()
{
	Super::NativeConstruct();

	if (RankList)
	{
		int32 Count = RankList->GetChildrenCount();
		RankLines.Empty();
		RankLines.Reserve(Count);

		for (int32 i = 0; i < Count; ++i)
		{
			UUserWidget_RankLine* RankLine = Cast<UUserWidget_RankLine>(RankList->GetChildAt(i));
			if (RankLine)
			{
				RankLines.Add(RankLine);
			}
		}
	}

	// 랭킹 리스트 초기화(게임 모드에서 받아오기)
	UWorld* World = GetWorld();
	AActionGameMode* GameMode = World->GetAuthGameMode<AActionGameMode>();
	const TArray<FRankData>& Rank = GameMode->GetRankDataArray();
	InitializeRankList(Rank);
}

void UUserWidget_Ranking::InitializeRankList(const TArray<FRankData>& Rank)
{
	int32 Count = RankLines.Num();

	for (int32 i = 0; i < Count; ++i)
	{
		RankLines[i]->SetDefault(i + 1, Rank[i].RankName, Rank[i].Gold);
	}
}
