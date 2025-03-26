// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_Ranking.h"
#include "Components/ScrollBox.h"
#include "UserWidget_RankLine.h"

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

	// 랭킹 리스트 초기화
	InitializeDefaultRankList();
}

void UUserWidget_Ranking::InitializeDefaultRankList()
{
	int32 Count = RankLines.Num();

	char BaseChar = 'A';

	FText RankerName;
	for (int32 i = 0; i < Count; ++i)
	{
		RankLines[i]->SetDefault(
			i+1, 
			FText::FromString(FString::Printf(TEXT("%c%c%c"), BaseChar + i, BaseChar + i, BaseChar + i)),
			(Count - i) * 1000000);
	}
}
