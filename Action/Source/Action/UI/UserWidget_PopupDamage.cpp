// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_PopupDamage.h"
#include "Components/TextBlock.h"

void UUserWidget_PopupDamage::PlayPopupAnimation(float Damage)
{
	if (DamageText)
	{
		FNumberFormattingOptions NumberFormat;
		NumberFormat.MinimumFractionalDigits = 0;
		NumberFormat.MaximumFractionalDigits = 0;
		NumberFormat.SetUseGrouping(true);
		
		DamageText->SetText(FText::AsNumber(Damage, &NumberFormat));
	}
	if (Popup)
	{
		PlayAnimation(Popup, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
}

void UUserWidget_PopupDamage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 항상 카메라를 바라보게 만들기

}
