// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_ClearPanel.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UUserWidget_ClearPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clear", meta = (BindWidget))
	class UUserWidget_Ranking* RankingPanel;
};
