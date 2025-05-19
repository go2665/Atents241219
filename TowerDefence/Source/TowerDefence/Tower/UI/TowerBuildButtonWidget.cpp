// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuildButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UTowerBuildButtonWidget::OnInitialize(int32 InIndex, UTexture* InImage, int32 InCost)
{
	Index = InIndex;
	Cost = InCost;

	if (BuildButton)
	{
		// 버튼 이미지 지정
		FButtonStyle ButtonStyle = BuildButton->GetStyle();
		FSlateBrush ImageBrush;
		ImageBrush.SetResourceObject(InImage);
		ButtonStyle.SetNormal(ImageBrush);
		ButtonStyle.SetHovered(ImageBrush);
		ButtonStyle.SetPressed(ImageBrush);
		ButtonStyle.SetDisabled(ImageBrush);
		BuildButton->SetStyle(ButtonStyle);

		// 버튼 클릭시 이벤트 바인딩
		BuildButton->OnClicked.AddDynamic(this, &UTowerBuildButtonWidget::OnButtonClicked);
	}
	if (PriceText)
	{
		// 가격 텍스트 지정
		PriceText->SetText(FText::AsNumber(Cost));
	}
}

void UTowerBuildButtonWidget::UpdateButtonState(int32 InGold)
{
	if (InGold >= Cost)
	{
		BuildButton->SetIsEnabled(true);
		PriceText->SetColorAndOpacity(BuildEnableColor);
	}
	else
	{
		BuildButton->SetIsEnabled(false);
		PriceText->SetColorAndOpacity(BuildDisableColor);
	}
}

void UTowerBuildButtonWidget::OnButtonClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("UTowerBuildButtonWidget : Mouse Clicked!"));
	// 자기 인덱스 알려주는 델리게이트 호출
	OnBuildButtonClicked.ExecuteIfBound(Index);
}
