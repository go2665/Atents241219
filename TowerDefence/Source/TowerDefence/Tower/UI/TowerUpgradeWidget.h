// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerUpgradeWidget.generated.h"

class ATower;
class ATowerDefenceGameMode;
class UButton;
class UTextBlock;
class UWidgetAnimation;

DECLARE_DYNAMIC_DELEGATE(FOnUpgradeClicked);
DECLARE_DYNAMIC_DELEGATE(FOnSellClicked);

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:	
	// 타워 업그레이드 위젯 초기화 함수
	void OnInitialize();

	// 타워 업그레이드 위젯 열기. 업그레이드 비용을 파라메터로 받음
	void OpenUpgradeWidget(int32 InUpgradeCost);
	
	// 타워 업그레이드 위젯 닫기
	void CloseUpgradeWidget();

private:
	// 업그레이드 버튼이 눌려졌을 때 호출되는 함수
	UFUNCTION()
	void UpgradeTower();

	// 판매 버튼이 눌려졌을 때 호출되는 함수
	UFUNCTION()
	void SellTower();
	// 닫기 애니메이션이 끝난 후 비지빌리티를 끄는 함수
	UFUNCTION()
	void OnCloseAnimationFinished();

	UFUNCTION()
	void UpdateCurrentGold(int32 InCurrentGold);

	// 업그레이드 버튼의 상태를 업데이트하는 함수(판매 가능/불가능 설정)
	void UpdateButtonState(int32 InUpgradeCost);

public:
	// 업그레이트 버튼이 눌려졌을 때 호출되는 델리게이트
	FOnUpgradeClicked OnUpgradeClicked;

	// 판매 버튼이 눌려졌을 때 호출되는 델리게이트
	FOnSellClicked OnSellClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* UpgradeButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UpgradeCostText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Open = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Close = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower")
	FLinearColor UpgradeEnableColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);		// 업그레이드 가능 색상

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower")
	FLinearColor UpgradeDisableColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);	// 업그레이드 불가능 색상

private:
	// 현재 GameMode에 저장되어 있는 골드
	int32 CurruntGold = 0;	
};
