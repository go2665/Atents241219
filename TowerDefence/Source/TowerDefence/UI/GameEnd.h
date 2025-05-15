// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEnd.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UGameEnd : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Open();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StartAnimation;
	
};
