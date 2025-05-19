// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerUpgradeWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"

void UTowerUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpgradeButton->OnClicked.AddDynamic(this, &UTowerUpgradeWidget::UpgradeTower);
	CloseButton->OnClicked.AddDynamic(this, &UTowerUpgradeWidget::SellTower);

	SetRenderScale(FVector2D(0.0f, 0.0f));		// Close 테스트 때문에 무조건 0으로 시작
	SetVisibility(ESlateVisibility::Hidden);	
		
	FWidgetAnimationDynamicEvent CloseEvent;
	CloseEvent.BindUFunction(this, FName("OnCloseAnimationFinished"));
	BindToAnimationFinished(Close, CloseEvent);	
}

void UTowerUpgradeWidget::OnInitialize()
{
	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
	CurruntGold = GameMode->GetGold();
	GameMode->OnGoldChanged.AddUObject(this, &UTowerUpgradeWidget::UpdateCurrentGold);	// 골드 변화 시 업데이트

	//GameMode->OnGoldChanged.RemoveAll(this);	// 이 객체에서 연결한 모든 델리게이트 해제
}

void UTowerUpgradeWidget::OpenUpgradeWidget(int32 InUpgradeCost)
{
	// Open 애니메이션 재생
	//UE_LOG(LogTemp, Warning, TEXT("Open Upgrade Widget"));

	UpdateButtonState(InUpgradeCost);	// 현재 돈 상태 확인해서 버튼 활성화/비활성화
	SetRenderScale(FVector2D(0.0f, 0.0f));
	SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(Open);	// 확대 애니메이션 재생
}

void UTowerUpgradeWidget::OnCloseAnimationFinished()
{
	// 다 작아지면 안보이게 설정
	SetVisibility(ESlateVisibility::Hidden);
	//UE_LOG(LogTemp, Warning, TEXT("Close Animation Finished"));
}

void UTowerUpgradeWidget::UpdateCurrentGold(int32 InCurrentGold)
{
	CurruntGold = InCurrentGold;	// 게임 모드에 골드 변화가 있으면 기록해 놓기
	//UE_LOG(LogTemp, Warning, TEXT("Current Gold : %d"), CurruntGold);
}

void UTowerUpgradeWidget::UpgradeTower()
{
	//UE_LOG(LogTemp, Warning, TEXT("UTowerUpgradeWidget::UpgradeTower : Mouse Clicked!"));

	OnUpgradeClicked.ExecuteIfBound();	// 업그레이드 버튼 클릭 시 델리게이트 실행
	CloseUpgradeWidget();				// 업그레이드 위젯 닫기
}

void UTowerUpgradeWidget::SellTower()
{
	//UE_LOG(LogTemp, Warning, TEXT("UTowerUpgradeWidget::SellTower : Mouse Clicked!"));

	//UE_LOG(LogTemp, Warning, TEXT("Tower Sell button clicked!"));
	OnSellClicked.ExecuteIfBound();	// 판매 버튼 클릭 시 델리게이트 실행
	CloseUpgradeWidget();			// 업그레이드 위젯 닫기
}

void UTowerUpgradeWidget::CloseUpgradeWidget()
{
	if (GetRenderTransform().Scale.X > 0)	// 열려있으면
	{
		//UE_LOG(LogTemp, Warning, TEXT("Close Upgrade Widget"));
		PlayAnimation(Close);	// 닫는 애니메이션 실행
	}
}

void UTowerUpgradeWidget::UpdateButtonState(int32 InUpgradeCost)
{
	// 버튼 활성화 설정
	bool IsEnabled = CurruntGold >= InUpgradeCost ? true : false;
	UpgradeButton->SetIsEnabled(IsEnabled);	

	// 텍스트 설정(금액, 색상)
	UpgradeCostText->SetText(FText::AsNumber(InUpgradeCost));
	if (IsEnabled)
	{
		UpgradeCostText->SetColorAndOpacity(UpgradeEnableColor);
	}
	else
	{
		UpgradeCostText->SetColorAndOpacity(UpgradeDisableColor);
	}
}
