// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
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
			HealthBar->SetPercent(InHealthRatio);
		}
	}
	inline void SetGoldText(int32 InGold)
	{
		if (GoldText)
		{
			GoldText->SetText(FText::AsNumber(InGold));
		}
	}

protected:
	UPROPERTY(meta = (BindWidget))
	UHeroTowerWidget* HeroTowerBuilder = nullptr;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText = nullptr;
};
