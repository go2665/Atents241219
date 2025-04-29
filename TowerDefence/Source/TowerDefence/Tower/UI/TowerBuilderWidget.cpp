// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuilderWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "TowerBuildButtonWidget.h"


void UTowerBuilderWidget::OnInitialize(const TArray<UTowerDataAsset*>* InTowerDatas)
{
	TowerDatas = InTowerDatas;

	// 반지름은 Canvas 크기의 절반
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas->Slot);
	FVector2D Size = CanvasSlot->GetSize();
	float Radius = FMath::Min(Size.X, Size.Y) * 0.5f;

	int32 Count = TowerDatas->Num();
	if (Canvas && Count > 0)
	{
		const FVector2D CanvasCenter = Canvas->GetRenderTransform().Translation;

		for (int32 i = 0; i < Count; i++)
		{
			// 각도 계산
			float RadianAngle = FMath::DegreesToRadians(i * (360.0f / Count));

			// 위치 좌표 구하기
			float X = FMath::Cos(RadianAngle) * Radius;
			float Y = FMath::Sin(RadianAngle) * Radius;

			// 버튼 생성
			UTowerBuildButtonWidget* TowerButtonWidget =
				CreateWidget<UTowerBuildButtonWidget>(this, TowerButtonButtonWidgetClass);
			TowerButtonsToIndexMap.Add(TowerButtonWidget, i);

			UCanvasPanelSlot* ChildCanvasSlot = Canvas->AddChildToCanvas(TowerButtonWidget);
			ChildCanvasSlot->SetPosition(CanvasCenter + FVector2D(X, Y));

			TowerButtonWidget->OnInitialize((*InTowerDatas)[i]->TowerImage, (*InTowerDatas)[i]->TowerCost);
			TowerButtonWidget->GetBuildButton()->OnClicked.AddDynamic(
				this, &UTowerBuilderWidget::OnBuildButtonClicked);
		}
	}
}

void UTowerBuilderWidget::OnBuildButtonClicked()
{
	//UTowerBuildButtonWidget* ButtonWidget = Cast<UTowerBuildButtonWidget>(GetFocusedWidget());
	//if (ButtonWidget)
	//{
	//	int32 Index = TowerButtonsToIndexMap.FindRef(ButtonWidget);
	//	UE_LOG(LogTemp, Warning, TEXT("Clicked button index: %d"), Index);
	//}
}
