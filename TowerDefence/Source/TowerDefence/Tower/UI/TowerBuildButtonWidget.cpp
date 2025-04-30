// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuildButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UTowerBuildButtonWidget::OnInitialize(int32 InIndex, UTexture* InImage, int32 InCost)
{
	Index = InIndex;

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

		BuildButton->OnClicked.AddDynamic(this, &UTowerBuildButtonWidget::OnButtonClicked);
	}
	if (PriceText)
	{
		PriceText->SetText(FText::AsNumber(InCost));
	}
}

void UTowerBuildButtonWidget::OnButtonClicked()
{
	OnBuildButtonClicked.ExecuteIfBound(Index);
}
