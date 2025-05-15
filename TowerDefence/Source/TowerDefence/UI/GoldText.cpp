// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldText.h"

void UGoldText::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float Speed = FMath::Max((TargetValue - CurrentValue) * MinimumSpeed, MinimumSpeed);
	CurrentValue = FMath::FInterpConstantTo(CurrentValue, TargetValue, InDeltaTime, Speed);

	GoldText->SetText(FText::AsNumber(FMath::RoundToInt(CurrentValue)));
}
