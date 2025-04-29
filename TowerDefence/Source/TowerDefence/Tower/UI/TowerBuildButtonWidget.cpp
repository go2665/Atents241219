// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuildButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UTowerBuildButtonWidget::OnInitialize(UTexture* InImage, int32 InCost)
{
	if (BuildButton)
	{
		FButtonStyle ButtonStyle = BuildButton->GetStyle();
		FSlateBrush ImageBrush;
		ImageBrush.SetResourceObject(InImage);
		ButtonStyle.SetNormal(ImageBrush);
		ButtonStyle.SetHovered(ImageBrush);
		ButtonStyle.SetPressed(ImageBrush);
		ButtonStyle.SetDisabled(ImageBrush);
		BuildButton->SetStyle(ButtonStyle);
	}
	if (PriceText)
	{
		PriceText->SetText(FText::AsNumber(InCost));
	}
}
