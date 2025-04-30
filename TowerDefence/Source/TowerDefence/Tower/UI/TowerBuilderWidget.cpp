// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuilderWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "TowerBuildButtonWidget.h"


void UTowerBuilderWidget::OnInitialize(const TArray<UTowerDataAsset*>* InTowerDatas)
{
	TowerDatas = InTowerDatas;

	// 반지름은 크기박스 크기의 절반
	float Radius = SizeBox->GetWidthOverride() * 0.5f;

	int32 Count = TowerDatas->Num();
	if (Canvas && Count > 0)
	{
		for (int32 i = 0; i < Count; i++)
		{
			// 각도 계산
			float RadianAngle = FMath::DegreesToRadians(i * (360.0f / Count) - 90.0f);

			// 위치 좌표 구하기
			float X = FMath::Cos(RadianAngle) * Radius;
			float Y = FMath::Sin(RadianAngle) * Radius;

			// 버튼 생성
			UTowerBuildButtonWidget* TowerButtonWidget =
				CreateWidget<UTowerBuildButtonWidget>(this, TowerButtonButtonWidgetClass);
						
			UCanvasPanelSlot* ChildCanvasSlot = Canvas->AddChildToCanvas(TowerButtonWidget);
			ChildCanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));		// 앵커와 피봇을 중앙으로 설정
			ChildCanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));

			ChildCanvasSlot->SetPosition(FVector2D(X, Y));
			//UE_LOG(LogTemp, Warning, TEXT("ChildCanvasSlot Position : [%.1f, %.1f]"), X, Y);

			ChildCanvasSlot->SetSize(TowerButtonWidget->GetButtonSize());

			TowerButtonWidget->OnInitialize(i, (*InTowerDatas)[i]->TowerImage, (*InTowerDatas)[i]->TowerCost);
			TowerButtonWidget->OnBuildButtonClicked.BindUFunction(this, FName("OnBuildButtonClicked"));	
		}
	}
}

void UTowerBuilderWidget::OnBuildButtonClicked(int32 InIndex)
{
	// 버튼이 클릭되면 TowerBuilder에게 요청 전달
	OnTowerBuildRequest.ExecuteIfBound(InIndex);
}
