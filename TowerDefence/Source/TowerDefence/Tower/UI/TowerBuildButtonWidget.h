// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerBuildButtonWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerBuildButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnInitialize(UTexture* InImage, int32 InCost);

	inline UButton* GetBuildButton() const { return BuildButton; }

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BuildButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PriceText = nullptr;	
};
