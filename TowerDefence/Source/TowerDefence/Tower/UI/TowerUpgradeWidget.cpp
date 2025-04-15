// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerUpgradeWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "TowerDefence/Tower/TowerBaseActor.h"
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

void UTowerUpgradeWidget::UpgradeWidgetInitialize(ATowerBaseActor* Tower)
{
	ensure(Tower);
	OwnerTower = Tower;	
	GameMode = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
}

void UTowerUpgradeWidget::OpenUpgradeWidget()
{
	// Open 애니메이션 재생
	//UE_LOG(LogTemp, Warning, TEXT("[%s] : Open Upgrade Widget"), *OwnerTower->GetActorNameOrLabel());

	UpdateButtonState();	// 현재 돈 상태 확인해서 버튼 활성화/비활성화
	SetRenderScale(FVector2D(0.0f, 0.0f));
	SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(Open);	// 확대 애니메이션 재생
}

void UTowerUpgradeWidget::Test_PrintTower()
{
	if (OwnerTower)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Tower: %s"), *OwnerTower->GetActorNameOrLabel());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner Tower is null"));
	}
}

void UTowerUpgradeWidget::SellTower()
{
	if (OwnerTower)
	{
		OwnerTower->Sell();		// 타워 판매 시도
	}
}

void UTowerUpgradeWidget::UpgradeTower()
{
	if (OwnerTower)
	{
		OwnerTower->LevelUp();	// 타워 레벨업 시도
	}
	CloseUpgradeWidget();		// 업그레이드 위젯 닫기
}

void UTowerUpgradeWidget::UpdateButtonState()
{
	// GameMode에 있는 돈을 확인해서 UpgradeButton 활성 및 비활성화
	bool IsEnabled = GameMode->GetGold() >= OwnerTower->GetCurrentUpgradeCost() ? true : false;
	UpgradeButton->SetIsEnabled(IsEnabled);	

	// 텍스트 설정(금액, 색상)
	UpgradeCostText->SetText(FText::AsNumber(OwnerTower->GetCurrentUpgradeCost()));
	if (IsEnabled)
	{
		UpgradeCostText->SetColorAndOpacity(UpgradeEnableColor);
	}
	else
	{
		UpgradeCostText->SetColorAndOpacity(UpgradeDisableColor);
	}
}

void UTowerUpgradeWidget::OnCloseAnimationFinished()
{
	SetVisibility(ESlateVisibility::Hidden);
	//UE_LOG(LogTemp, Warning, TEXT("[%s] : Close Animation Finished"), *OwnerTower->GetActorNameOrLabel());
}

void UTowerUpgradeWidget::CloseUpgradeWidget()
{
	if (GetRenderTransform().Scale.X > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[%s] : Close Upgrade Widget"), *OwnerTower->GetActorNameOrLabel());
		PlayAnimation(Close);
	}
}
