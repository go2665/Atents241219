// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthRatioBar.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UHealthRatioBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	inline void SetRatio(float InValue) { TargetValue = InValue; }

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	// 비율 변경 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthBar")
	float ChangeSpeed = 3.0f;

private:
	float CurrentValue = 0;
	float TargetValue = 0;
};
