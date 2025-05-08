// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

protected:
	UPROPERTY(meta = (BindWidget))
	UHeroTowerWidget* HeroTowerBuilder = nullptr;
};
