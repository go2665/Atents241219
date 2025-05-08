// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceHUD.h"
#include "TowerDefence/UI/HeroTowerWidget.h"
#include "TowerDefence/UI/MainWidget.h"

void ATowerDefenceHUD::BeginPlay()
{
	Super::BeginPlay();
	if (MainWidgetClass)
	{
		UMainWidget* MainWidget = CreateWidget<UMainWidget>(GetWorld(), MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
			HeroTowerBuilder = MainWidget->GetHeroTowerBuilder();
			HeroTowerBuilder->OnSetup();
			HeroTowerBuilder->OnTowerBuildRequest.BindLambda(
				[this](int32 InIndex)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hero Tower Build Request! Index : %d"), InIndex);
					LastClickedTowerIndex = InIndex;	// 클릭한 타워 인덱스 저장
				}
			);
		}
	}
}
