// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_RankLine.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Action/Framework/ActionGameMode.h"

void UUserWidget_RankLine::SetDefault(int32 InRank, FText InName, int32 InGold)
{
	RankIndex = InRank - 1;
	// InRank : 1st, 2nd, 3rd, 4th, 5th ...
	if (InRank > 3)
	{
		Rank->SetText(FText::FromString(FString::Printf(TEXT("%dth"), InRank)));
	}
	else
	{
		static const FString RankStrings[] = { "1st", "2nd", "3rd" };
		Rank->SetText(FText::FromString(RankStrings[InRank - 1]));
	}
	
	RankerName->SetText(InName);
	Gold->SetText(FText::AsNumber(InGold));
}

void UUserWidget_RankLine::SetRank(FString InName, int32 InGold)
{
	RankerName->SetText(FText::FromString(InName));
	Gold->SetText(FText::AsNumber(InGold));
}

void UUserWidget_RankLine::OnNewRanker()
{
	// 이름 입력이 가능하게 스위처를 변경
	Switcher->SetActiveWidgetIndex(1);
	NameInput->SetFocus();
}

void UUserWidget_RankLine::NativeConstruct()
{
	Super::NativeConstruct();
	Switcher->SetActiveWidgetIndex(0);
	NameInput->OnTextCommitted.AddDynamic(this, &UUserWidget_RankLine::OnNameCommitted);

	UWorld* World = GetWorld();
	AActionGameMode* GameMode = World->GetAuthGameMode<AActionGameMode>();
	OnRankerNameCommitted.AddDynamic(GameMode, &AActionGameMode::SetRankerName);
}

void UUserWidget_RankLine::OnNameCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (InCommitMethod == ETextCommit::OnEnter)
	{
		Switcher->SetActiveWidgetIndex(0);
		RankerName->SetText(InText);

		OnRankerNameCommitted.Broadcast(RankIndex, InText);
	}
	
}
