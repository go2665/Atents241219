// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_RankLine.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

void UUserWidget_RankLine::SetDefault(int32 InRank, FText InName, int32 InGold)
{
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

void UUserWidget_RankLine::OnNewRanker(int32 InNewGold)
{
	Switcher->SetActiveWidgetIndex(1);
	Gold->SetText(FText::AsNumber(InNewGold));
	NameInput->SetFocus();
}

void UUserWidget_RankLine::NativeConstruct()
{
	Super::NativeConstruct();
	Switcher->SetActiveWidgetIndex(0);
	NameInput->OnTextCommitted.AddDynamic(this, &UUserWidget_RankLine::OnNameCommitted);
}

void UUserWidget_RankLine::OnNameCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	Switcher->SetActiveWidgetIndex(0);
	RankerName->SetText(InText);
}
