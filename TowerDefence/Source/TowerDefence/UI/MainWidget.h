// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "TowerDefence/UI/GoldText.h"
#include "TowerDefence/UI/HealthRatioBar.h"
#include "MainWidget.generated.h"

class UHeroTowerWidget;
/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	inline UHeroTowerWidget* GetHeroTowerBuilder() const { return HeroTowerBuilder; }
	inline void SetHealthRatio(float InHealthRatio)
	{
		if (HealthBar)
		{
			HealthBar->SetRatio(InHealthRatio);
		}
	}
	inline void SetGoldText(int32 InGold)
	{
		if (GoldText)
		{
			GoldText->SetGold(InGold);
		}
	}

protected:
	UPROPERTY(meta = (BindWidget))
	UHeroTowerWidget* HeroTowerBuilder = nullptr;

	UPROPERTY(meta = (BindWidget))
	UHealthRatioBar* HealthBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	UGoldText* GoldText = nullptr;
};
