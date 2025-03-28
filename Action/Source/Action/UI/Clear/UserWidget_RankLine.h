// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_RankLine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRankerNameCommitted, int32, InRankIndex, const FText&, InText);


/**
 * 
 */
UCLASS()
class ACTION_API UUserWidget_RankLine : public UUserWidget
{
	GENERATED_BODY()

public:
	// 디폴트 설정하기
	void SetDefault(int32 InRank, FText InName, int32 InGold);

	// 랭킹 설정하기
	void SetRank(FString InName, int32 InGold);

	// 입력 가능하게 하기
	void OnNewRanker();


protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnNameCommitted(const FText& InText, ETextCommit::Type InCommitMethod);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank", meta = (BindWidget))
	class UTextBlock* Rank;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank", meta = (BindWidget))
	class UWidgetSwitcher* Switcher;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank", meta = (BindWidget))
	class UTextBlock* RankerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank", meta = (BindWidget))
	class UEditableTextBox* NameInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rank", meta = (BindWidget))
	class UTextBlock* Gold;

private:
	FOnRankerNameCommitted OnRankerNameCommitted;
	int32 RankIndex = -1;
	
};
