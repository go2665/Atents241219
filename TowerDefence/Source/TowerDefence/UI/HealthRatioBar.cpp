// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthRatioBar.h"

void UHealthRatioBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (HealthBar)
	{
		CurrentValue = FMath::FInterpTo(CurrentValue, TargetValue, InDeltaTime, ChangeSpeed);
		HealthBar->SetPercent(CurrentValue);
	}
}
