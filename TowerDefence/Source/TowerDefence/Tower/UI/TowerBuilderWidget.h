// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Tower/Data/TowerDataAsset.h"
#include "TowerBuildButtonWidget.h"
#include "TowerBuilderWidget.generated.h"


//class UTowerBuildButtonWidget;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerBuilderWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void OnInitialize(const TArray<UTowerDataAsset*>* InTowerDatas);

private:
	UFUNCTION()
	void OnBuildButtonClicked();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TowerBuilder")
	TSubclassOf<UTowerBuildButtonWidget> TowerButtonButtonWidgetClass;

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas = nullptr;

private:
	UPROPERTY()
	TMap<UTowerBuildButtonWidget*, int32> TowerButtonsToIndexMap;
		
	const TArray<UTowerDataAsset*>* TowerDatas = nullptr;

};
