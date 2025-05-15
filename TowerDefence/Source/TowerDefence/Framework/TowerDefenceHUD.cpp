// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceHUD.h"
#include "TowerDefence/UI/HeroTowerWidget.h"
#include "TowerDefence/UI/MainWidget.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"

void ATowerDefenceHUD::BeginPlay()
{
	Super::BeginPlay();
	if (MainWidgetClass)
	{
		UMainWidget* MainWidget = CreateWidget<UMainWidget>(GetWorld(), MainWidgetClass);
		if (MainWidget)
		{
			ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(GetWorld()->GetAuthGameMode());
			if (GameMode)
			{
				GameMode->OnGoldChanged.AddUObject(MainWidget, &UMainWidget::SetGoldText);
				GameMode->OnHealthChanged.AddUObject(MainWidget, &UMainWidget::SetHealthRatio);
				GameMode->OnGameClear.AddUObject(MainWidget, &UMainWidget::OnGameClear);
				GameMode->OnGameOver.AddUObject(MainWidget, &UMainWidget::OnGameOver);

				MainWidget->SetGoldText(GameMode->GetGold());
				MainWidget->SetHealthRatio(1.0f);
			}

			MainWidget->AddToViewport();
			HeroTowerBuilder = MainWidget->GetHeroTowerBuilder();
			HeroTowerBuilder->OnSetup();
		}
	}
}
