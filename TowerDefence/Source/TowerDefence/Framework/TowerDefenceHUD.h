// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TowerDefenceHUD.generated.h"

class UMainWidget;
class UHeroTowerWidget;
/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API ATowerDefenceHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UMainWidget> MainWidgetClass;

private:
	UPROPERTY()
	UHeroTowerWidget* HeroTowerBuilder = nullptr;

	// 마지막으로 클릭한 타워 인덱스
	int32 LastClickedTowerIndex = -1;	
};
