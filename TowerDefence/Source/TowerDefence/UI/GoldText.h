// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GoldText.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UGoldText : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	inline void SetGold(int32 InValue) { TargetValue = InValue;	}

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText = nullptr;	

	// 텍스트 변경 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoldText")
	float MinimumSpeed = 10.0f;

private:
	float CurrentValue = 0;
	int32 TargetValue = 0;
};
